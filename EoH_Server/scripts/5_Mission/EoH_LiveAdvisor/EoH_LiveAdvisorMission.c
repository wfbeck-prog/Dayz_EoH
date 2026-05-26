modded class MissionServer
{
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

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_WriteLiveAdvisorHeartbeat, heartbeatSeconds * 1000, true);
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
