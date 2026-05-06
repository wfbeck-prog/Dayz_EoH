modded class MissionServer
{
    protected bool m_EoH_ServerInitialized;
    protected bool m_EoH_DT_LiveUpdatesStarted;
    protected bool m_EoH_RelaysSpawned;

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

            EoH_MarkerData data = new EoH_MarkerData("EoH_TOWN_BASE_" + town, town, cfg.GetRelayVector());
            data.Category = EoH_MarkerCategory.TOWN;
            data.State = EoH_MarkerState.NORMAL;
            data.Icon = "Territory";
            data.Is3D = 0;
            data.Pulse = 0;
            data.Color = ARGB(120, 150, 150, 150);
            data.BaseColor = data.Color;
            data.Normalize();

            EoH_MarkerService.SendToPlayer(player, data);
        }

        Print("[EoH] Sent base markers to player: " + player.GetIdentity().GetName());
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

        foreach (EoH_RelayLocation relay : cfg.Relays)
        {
            if (!relay || !relay.Enabled)
                continue;

            vector pos = relay.GetPosition();
            if (pos == "0 0 0".ToVector())
                continue;

            pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.2;

            if (EoH_RelayAlreadyExists(pos, 2.0))
                continue;

            Object obj = GetGame().CreateObjectEx("EoH_RadioRelay", pos, ECE_PLACE_ON_SURFACE);
            if (!obj)
            {
                Print("[EoH] Failed to spawn relay for " + relay.TownName + " at " + pos.ToString());
                continue;
            }

            obj.SetOrientation(Vector(relay.Orientation, 0, 0));
            Print("[EoH] Spawned relay for " + relay.TownName + " at " + pos.ToString());
        }

        m_EoH_RelaysSpawned = true;
    }

    bool EoH_RelayAlreadyExists(vector pos, float radius)
    {
        array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition3D(pos, radius, objects, null);

        foreach (Object obj : objects)
        {
            if (obj && obj.GetType() == "EoH_RadioRelay")
                return true;
        }

        return false;
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

        EoH_CaptureManager captureManager = EoH_CaptureManager.Get();
        if (captureManager)
            captureManager.Tick();

        EoH_RT_TraderManager traderManager = EoH_RT_TraderManager.Get();
        if (traderManager)
            traderManager.Update();
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
