modded class MissionServer
{
    protected ref Timer m_EoHLiveAdvisorHeartbeatTimer;

    override void OnInit()
    {
        super.OnInit();

        EoH_LiveAdvisorLogger.Init();
        EoH_LiveAdvisorLogger.Log("MISSION_INIT", "MissionServer initialized Live Advisor", "info", "MissionServer");
        EoH_StartLiveAdvisorHeartbeat();
    }

    void EoH_StartLiveAdvisorHeartbeat()
    {
        if (!EoH_LiveAdvisorLogger.m_Config || !EoH_LiveAdvisorLogger.m_Config.Enabled)
        {
            return;
        }

        int heartbeatSeconds = EoH_LiveAdvisorLogger.m_Config.HeartbeatSeconds;

        if (heartbeatSeconds < 60)
        {
            heartbeatSeconds = 60;
        }

        m_EoHLiveAdvisorHeartbeatTimer = new Timer(CALL_CATEGORY_SYSTEM);
        m_EoHLiveAdvisorHeartbeatTimer.Run(heartbeatSeconds, this, "EoH_WriteLiveAdvisorHeartbeat", NULL, true);
    }

    void EoH_WriteLiveAdvisorHeartbeat()
    {
        if (!EoH_LiveAdvisorLogger.m_Config || !EoH_LiveAdvisorLogger.m_Config.Enabled)
        {
            return;
        }

        int playerCount = 0;

        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);

        if (players)
        {
            playerCount = players.Count();
        }

        string message = string.Format("Server heartbeat. Players online: %1", playerCount);
        EoH_LiveAdvisorLogger.Log("SERVER_HEARTBEAT", message, "info", "MissionServer");
    }
}
