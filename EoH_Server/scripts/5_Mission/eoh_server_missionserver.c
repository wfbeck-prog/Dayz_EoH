modded class MissionServer
{
    protected bool m_EoH_ServerInitialized;
    protected bool m_EoH_DT_LiveUpdatesStarted;
    protected bool m_EoH_RelaysSpawned;
    protected int m_EoH_LastCaptureTick;
    protected int m_EoH_LastTraderTick;
    protected int m_EoH_LastAtmosphereTick;

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
                data = EoH_TownMarkerManager.BuildTownMarker(town, owner, EoH_MarkerState.OWNED, 0, EoH_TownMarkerManager.GetGroupColor(owner));
            else
                data = EoH_TownMarkerManager.BuildTownMarker(town, "Unclaimed", EoH_MarkerState.NORMAL, 0, ARGB(120, 150, 150, 150));

            data.Label = town;
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
        GetEoHAtmosphereConfig();
        EoH_WorldStateManager.Get();
        EoH_AIManager.Get();
        EoH_CaptureManager.Get();
        EoH_RT_TraderManager.Get().Initialize();
        EoH_AtmosphereManager.Get();
        EoH_DiscordWebhook.GetConfig();

        EoH_InitTownMarkers();
        EoH_SpawnRelaysFromConfig();

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_Server_Tick, 1000, true);

        m_EoH_ServerInitialized = true;
    }

    void EoH_SpawnRelaysFromConfig()
    {
        if (m_EoH_RelaysSpawned)
            return;

        EoH_RelayConfig cfg = GetEoHRelayConfig();
        if (!cfg || !cfg.Enabled || !cfg.SpawnRelaysOnServerStart)
            return;

        ref set<string> spawnedKeys = new set<string>();

        foreach (EoH_RelayLocation relay : cfg.Relays)
        {
            if (!relay || !relay.Enabled)
                continue;

            vector pos = relay.GetPosition();
            if (pos == "0 0 0".ToVector())
                continue;

            pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.2;
            string key = EoH_RelaySpawnKey(pos);

            if (spawnedKeys.Find(key) != -1)
            {
                Print("[EoH] Skipping duplicate relay config position for " + relay.TownName + " at " + pos.ToString());
                continue;
            }

            if (cfg.DeleteExistingEoHRelaysBeforeSpawn)
                EoH_DeleteExistingRelaysNear(pos, 15.0);
            else if (EoH_RelayAlreadyExists(pos, 15.0))
            {
                Print("[EoH] Relay already exists near " + relay.TownName + " at " + pos.ToString());
                spawnedKeys.Insert(key);
                continue;
            }

            Object obj = GetGame().CreateObjectEx("EoH_RadioRelay", pos, ECE_PLACE_ON_SURFACE);
            if (!obj)
            {
                Print("[EoH] Failed to spawn relay for " + relay.TownName + " at " + pos.ToString());
                continue;
            }

            obj.SetOrientation(Vector(relay.Orientation, 0, 0));
            spawnedKeys.Insert(key);
            Print("[EoH] Spawned relay for " + relay.TownName + " at " + pos.ToString());
        }

        m_EoH_RelaysSpawned = true;
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

    bool EoH_RelayAlreadyExists(vector pos, float radius)
    {
        array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition3D(pos, radius, objects, null);

        foreach (Object obj : objects)
        {
            if (obj && EoH_IsRelayType(obj.GetType()))
                return true;
        }

        return false;
    }

    void EoH_DeleteExistingRelaysNear(vector pos, float radius)
    {
        array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition3D(pos, radius, objects, null);

        foreach (Object obj : objects)
        {
            if (!obj)
                continue;

            string type = obj.GetType();
            if (!EoH_IsRelayType(type))
                continue;

            Print("[EoH] Removing existing relay before spawn: " + type + " at " + obj.GetPosition().ToString());
            GetGame().ObjectDelete(obj);
        }
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

        if (now - m_EoH_LastAtmosphereTick >= 30000)
        {
            m_EoH_LastAtmosphereTick = now;
            EoH_AtmosphereManager atmosphereManager = EoH_AtmosphereManager.Get();
            if (atmosphereManager)
                atmosphereManager.Tick();
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
