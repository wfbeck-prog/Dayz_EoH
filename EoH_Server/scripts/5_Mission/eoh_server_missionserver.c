modded class MissionServer
{
    protected bool m_EoH_ServerInitialized;
    protected bool m_EoH_DT_LiveUpdatesStarted;

    override void OnInit()
    {
        super.OnInit();

        EoH_Server_Init();
        EoH_DT_StartLiveUpdates();
    }

    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);

        EoH_DT_UpdatePlayerDogtag(player);
    }

    protected void EoH_Server_Init()
    {
        if (m_EoH_ServerInitialized)
            return;

        Print("[EoH_Server] Initializing core systems...");

        EoH_WorldStateManager.Get();
        EoH_AIManager.Get();
        EoH_CaptureManager.Get();

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_Server_Tick, 1000, true);

        m_EoH_ServerInitialized = true;

        Print("[EoH_Server] Core systems initialized.");
    }

    protected void EoH_DT_StartLiveUpdates()
    {
        if (m_EoH_DT_LiveUpdatesStarted)
            return;

        if (!GetGame() || !GetGame().IsServer())
            return;

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_DT_UpdateAllPlayerDogtags, 60000, true);
        m_EoH_DT_LiveUpdatesStarted = true;

        Print("[EoH_DogtagBridge] Live dogtag updates active.");
    }

    void EoH_Server_Tick()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        EoH_CaptureManager captureManager = EoH_CaptureManager.Get();
        if (captureManager)
            captureManager.Tick();
    }

    void EoH_DT_UpdateAllPlayerDogtags()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

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
        if (!player || !player.GetIdentity())
            return;

        int score = EoH_DT_TerjeAdapter.GetBestSkillScore(player);
        string desired = EoH_DT_Core.ResolveDogtagClass(score);

        if (desired == "")
            return;

        player.ReplaceDogtag(desired);
    }
};
