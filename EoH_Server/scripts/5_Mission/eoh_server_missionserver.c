modded class MissionServer
{
    protected bool m_EoH_ServerInitialized;
    protected bool m_EoH_DT_LiveUpdatesStarted;

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
        EoH_WorldStateManager.Get();
        EoH_AIManager.Get();
        EoH_CaptureManager.Get();
        EoH_RT_TraderManager.Get().Initialize();

        EoH_InitTownMarkers();

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_Server_Tick, 1000, true);

        m_EoH_ServerInitialized = true;
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
