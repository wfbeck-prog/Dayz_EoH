class EoH_LiveAdvisorPerformance
{
    protected static float m_LastPerformanceWarningTime;
    protected static float m_SampleTimer;

    static void CheckServerPerformance(float timeslice)
    {
        if (!EoH_LiveAdvisorLogger.m_Config)
            return;

        if (!EoH_LiveAdvisorLogger.m_Config.EnablePerformanceMonitor)
            return;

        m_SampleTimer += timeslice;

        if (m_SampleTimer < EoH_LiveAdvisorLogger.m_Config.PerformanceSampleSeconds)
            return;

        m_SampleTimer = 0;

        float fps = 0;

        if (timeslice > 0)
        {
            fps = 1.0 / timeslice;
        }

        int players = EoH_GetPlayerCount();

        if (EoH_LiveAdvisorLogger.m_Config.LogPerformanceSamples)
        {
            string sampleMessage = "FPS=" + fps.ToString() + " Players=" + players.ToString() + " Timeslice=" + timeslice.ToString();
            EoH_LiveAdvisorLogger.Log("PERFORMANCE_SAMPLE", sampleMessage, "info", "EoH_Performance");
        }

        string activityMessage = "FPS=" + fps.ToString() + " Players=" + players.ToString();
        EoH_LiveAdvisorActivity.LogActivity("performance_monitor", activityMessage);

        float now = GetGame().GetTime() / 1000.0;
        float cooldown = EoH_LiveAdvisorLogger.m_Config.PerformanceWarningCooldownSeconds;

        if (m_LastPerformanceWarningTime > 0 && now - m_LastPerformanceWarningTime < cooldown)
        {
            return;
        }

        if (fps <= EoH_LiveAdvisorLogger.m_Config.CriticalFpsThreshold)
        {
            m_LastPerformanceWarningTime = now;

            string criticalMessage = "CRITICAL server FPS detected. FPS=" + fps.ToString() + " Timeslice=" + timeslice.ToString() + " ActivePlayers=" + players.ToString();

            EoH_LiveAdvisorLogger.Log("SERVER_FPS_CRITICAL", criticalMessage, "critical", "EoH_Performance");
            return;
        }

        if (fps <= EoH_LiveAdvisorLogger.m_Config.LowFpsWarningThreshold)
        {
            m_LastPerformanceWarningTime = now;

            string warningMessage = "Low server FPS detected. FPS=" + fps.ToString() + " Timeslice=" + timeslice.ToString() + " ActivePlayers=" + players.ToString();

            EoH_LiveAdvisorLogger.Log("SERVER_FPS_WARNING", warningMessage, "warning", "EoH_Performance");
        }
    }

    static int EoH_GetPlayerCount()
    {
        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);

        if (!players)
            return 0;

        return players.Count();
    }
}
