modded class MissionServer
{
    protected bool m_EoH_ServerInitialized;
    protected bool m_EoH_DT_LiveUpdatesStarted;
    protected bool m_EoH_RelayMaintenanceStarted;
    protected int m_EoH_LastCaptureTick;
    protected int m_EoH_LastTraderTick;
    protected int m_EoH_LastRelayMaintenanceTick;

    override void OnInit()
    {
        super.OnInit();

        EoH_Server_Init();
        EoH_DT_StartLiveUpdates();

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_Radio_Tick, 120000, true);
    }

    void EoH_Radio_Tick()
    {
        EoH_RadioIntel.Get().BroadcastFromTower("7075 0 7700".ToVector(), 2000);
        EoH_RadioIntel.Get().BroadcastFromTower("4800 0 9600".ToVector(), 2500);
    }

    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);
        EoH_DT_UpdatePlayerDogtag(player);
        SendBaseTownMarkers(player);
    }

    void SendBaseTownMarkers(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        EoH_CaptureManager cap = EoH_CaptureManager.Get();
        array<string> towns = cap.GetAllTownNames();

        foreach (string town : towns)
        {
            EoH_CaptureTownConfig cfg = cap.GetTownConfig(town);
            if (!cfg)
                continue;

            string oldBaseId = "EoH_TOWN_BASE_" + town;
            oldBaseId.Replace(" ", "_");
            EoH_MarkerService.RemoveFromPlayer(player, oldBaseId);

            string owner = cap.GetTownOwner(town);
            EoH_MarkerData data;

            if (owner != "")
            {
                data = EoH_TownMarkerManager.BuildTownMarker(town, owner, EoH_MarkerState.OWNED, 0, EoH_TownMarkerManager.GetGroupColor(owner));
                if (owner != "Unclaimed")
                    data.Label = town + " " + EoH_TownMarkerManager.GetGroupTag(owner);
                else
                    data.Label = town;
            }
            else
            {
                data = EoH_TownMarkerManager.BuildTownMarker(town, "Unclaimed", EoH_MarkerState.NORMAL, 0, ARGB(120, 150, 150, 150));
                data.Label = town;
            }

            data.Icon = "Territory";
            data.Normalize();

            EoH_MarkerService.SendToPlayer(player, data);
        }

        Print("[EoH] Sent unified town markers to player: " + player.GetIdentity().GetName());
    }

    protected void EoH_Server_Init()
    {
        if (m_EoH_ServerInitialized)
            return;

        GetEoHBuildControlConfig();
        GetEoHRelayConfig();
        EoH_WorldStateManager.Get();
        EoH_AIManager.Get();
        EoH_CaptureManager.Get();
        EoH_RT_TraderManager.Get().Initialize();
        EoH_DiscordWebhook.GetConfig();

        EoH_InitTownMarkers();
        EoH_StartRelayMaintenance();

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_Server_Tick, 1000, true);

        m_EoH_ServerInitialized = true;
    }

    void EoH_StartRelayMaintenance()
    {
        if (m_EoH_RelayMaintenanceStarted)
            return;

        m_EoH_RelayMaintenanceStarted = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_MaintainConfiguredRelays, 30000, false);
        Print("[EoH] Relay system is in maintenance mode. No startup spawning or purging will be performed.");
    }

    void EoH_MaintainConfiguredRelays()
    {
        EoH_RelayConfig cfg = GetEoHRelayConfig();
        if (!cfg || !cfg.Enabled)
            return;

        int found = 0;
        int missing = 0;

        foreach (EoH_RelayLocation relay : cfg.Relays)
        {
            if (!relay || !relay.Enabled)
                continue;

            vector pos = relay.GetPosition();
            if (pos == "0 0 0".ToVector())
                continue;

            Object obj = EoH_FindRelayNear(pos, 25.0);
            if (!obj)
            {
                missing++;
                Print("[EoH][WARN] No editor/persistent relay found near " + relay.TownName + " at " + pos.ToString() + ". Place it in DayZ Editor or update relay config.");
                continue;
            }

            found++;
            EoH_RefreshRelayObject(obj, relay.TownName);
        }

        Print("[EoH] Relay maintenance complete. found=" + found.ToString() + " missing=" + missing.ToString());
    }

    Object EoH_FindRelayNear(vector pos, float radius)
    {
        array<Object> objects = new array<Object>();
        vector searchPos = pos;
        searchPos[1] = GetGame().SurfaceY(pos[0], pos[2]);
        GetGame().GetObjectsAtPosition3D(searchPos, radius, objects, null);

        Object best;
        float bestDist = 999999.0;

        foreach (Object obj : objects)
        {
            if (!obj || !EoH_IsRelayType(obj.GetType()))
                continue;

            float dist = EoH_Distance2D(obj.GetPosition(), pos);
            if (dist < bestDist)
            {
                bestDist = dist;
                best = obj;
            }
        }

        return best;
    }

    float EoH_Distance2D(vector a, vector b)
    {
        float dx = a[0] - b[0];
        float dz = a[2] - b[2];
        return Math.Sqrt((dx * dx) + (dz * dz));
    }

    void EoH_RefreshRelayObject(Object obj, string townName)
    {
        if (!obj)
            return;

        EntityAI entity = EntityAI.Cast(obj);
        if (entity)
        {
            entity.SetLifetime(3888000);
            entity.SetHealth("", "", entity.GetMaxHealth("", ""));
            entity.SetSynchDirty();
        }

        Print("[EoH] Maintained relay town=" + townName + " type=" + obj.GetType() + " pos=" + obj.GetPosition().ToString());
    }

    string EoH_RelaySpawnKey(vector pos)
    {
        int x = Math.Round(pos[0]);
        int z = Math.Round(pos[2]);
        return x.ToString() + "_" + z.ToString();
    }

    bool EoH_IsRelayType(string type)
    {
        return type == "EoH_RadioRelay" || type == "EoH_CaptureRelay_Base" || type.Contains("EoH_RadioRelay") || type.Contains("EoH_CaptureRelay");
    }

    void EoH_InitTownMarkers()
    {
        EoH_CaptureManager cap = EoH_CaptureManager.Get();
        if (!cap)
            return;

        array<string> towns = cap.GetAllTownNames();

        foreach (string town : towns)
        {
            string owner = cap.GetTownOwner(town);

            if (owner != "")
                EoH_TownMarkerManager.UpdateTownMarker(town, owner);
        }
    }

    protected void EoH_DT_StartLiveUpdates()
    {
        if (m_EoH_DT_LiveUpdatesStarted)
            return;

        if (!GetGame() || !GetGame().IsServer())
            return;

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_DT_UpdateAllPlayerDogtags, 60000, true);
        m_EoH_DT_LiveUpdatesStarted = true;
    }

    void EoH_Server_Tick()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        int now = GetGame().GetTime();

        if (now - m_EoH_LastCaptureTick >= 5000)
        {
            m_EoH_LastCaptureTick = now;
            EoH_CaptureManager captureManager = EoH_CaptureManager.Get();
            if (captureManager)
                captureManager.Tick();
        }

        if (now - m_EoH_LastTraderTick >= 30000)
        {
            m_EoH_LastTraderTick = now;
            EoH_RT_TraderManager traderManager = EoH_RT_TraderManager.Get();
            if (traderManager)
                traderManager.Update();
        }

        if (now - m_EoH_LastRelayMaintenanceTick >= 900000)
        {
            m_EoH_LastRelayMaintenanceTick = now;
            EoH_MaintainConfiguredRelays();
        }
    }

    void EoH_DT_UpdateAllPlayerDogtags()
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity() || !player.IsAlive())
                continue;

            EoH_DT_UpdatePlayerDogtag(player);
        }
    }

    void EoH_DT_UpdatePlayerDogtag(PlayerBase player)
    {
        int score = EoH_DT_TerjeAdapter.GetBestSkillScore(player);
        string desired = EoH_DT_Core.ResolveDogtagClass(score);

        if (desired != "")
            player.ReplaceDogtag(desired);
    }
};
