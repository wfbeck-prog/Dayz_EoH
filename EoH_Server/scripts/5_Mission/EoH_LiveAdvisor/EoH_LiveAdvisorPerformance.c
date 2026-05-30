class EoH_LiveAdvisorPerformance
{
    protected static float m_LastPerformanceWarningTime;
    protected static float m_SampleTimer;
    protected static float m_AccumulatedTime;
    protected static int m_FrameCount;
    protected static float m_WorstTimeslice;

    static void CheckServerPerformance(float timeslice)
    {
        if (!EoH_LiveAdvisorLogger.m_Config)
            return;

        if (!EoH_LiveAdvisorLogger.m_Config.EnablePerformanceMonitor)
            return;

        m_SampleTimer += timeslice;
        m_AccumulatedTime += timeslice;
        m_FrameCount++;

        if (timeslice > m_WorstTimeslice)
        {
            m_WorstTimeslice = timeslice;
        }

        if (m_SampleTimer < EoH_LiveAdvisorLogger.m_Config.PerformanceSampleSeconds)
            return;

        float avgFps = 0;
        float worstFrameFps = 0;

        if (m_AccumulatedTime > 0 && m_FrameCount > 0)
        {
            avgFps = m_FrameCount / m_AccumulatedTime;
        }

        if (m_WorstTimeslice > 0)
        {
            worstFrameFps = 1.0 / m_WorstTimeslice;
        }

        int players = EoH_GetPlayerCount();

        if (EoH_LiveAdvisorLogger.m_Config.LogPerformanceSamples)
        {
            string sampleMessage = "AvgFPS=" + avgFps.ToString() + " WorstFrameFPS=" + worstFrameFps.ToString() + " WorstTimeslice=" + m_WorstTimeslice.ToString() + " Players=" + players.ToString() + " Frames=" + m_FrameCount.ToString();
            EoH_LiveAdvisorLogger.Log("PERFORMANCE_SAMPLE", sampleMessage, "info", "EoH_Performance");
        }

        string activityMessage = "AvgFPS=" + avgFps.ToString() + " WorstFrameFPS=" + worstFrameFps.ToString() + " Players=" + players.ToString();
        EoH_LiveAdvisorActivity.LogActivity("performance_monitor", activityMessage);

        EoH_AdvisorRecommendationManager.Get().ObservePerformanceSample(m_WorstTimeslice, avgFps, players);

        float now = GetGame().GetTime() / 1000.0;
        float cooldown = EoH_LiveAdvisorLogger.m_Config.PerformanceWarningCooldownSeconds;

        if (m_LastPerformanceWarningTime <= 0 || now - m_LastPerformanceWarningTime >= cooldown)
        {
            if (avgFps <= EoH_LiveAdvisorLogger.m_Config.CriticalFpsThreshold)
            {
                m_LastPerformanceWarningTime = now;
                string criticalMessage = "CRITICAL sustained low FPS detected. AvgFPS=" + avgFps.ToString() + " WorstFrameFPS=" + worstFrameFps.ToString() + " WorstTimeslice=" + m_WorstTimeslice.ToString() + " ActivePlayers=" + players.ToString();
                EoH_LiveAdvisorLogger.Log("SERVER_FPS_CRITICAL", criticalMessage, "critical", "EoH_Performance");
            }
            else if (avgFps <= EoH_LiveAdvisorLogger.m_Config.LowFpsWarningThreshold)
            {
                m_LastPerformanceWarningTime = now;
                string warningMessage = "Low sustained server FPS detected. AvgFPS=" + avgFps.ToString() + " WorstFrameFPS=" + worstFrameFps.ToString() + " WorstTimeslice=" + m_WorstTimeslice.ToString() + " ActivePlayers=" + players.ToString();
                EoH_LiveAdvisorLogger.Log("SERVER_FPS_WARNING", warningMessage, "warning", "EoH_Performance");
            }
            else if (m_WorstTimeslice >= EoH_LiveAdvisorLogger.m_Config.FrameHitchCriticalTimeslice)
            {
                m_LastPerformanceWarningTime = now;
                string hitchCriticalMessage = "Critical frame hitch detected. WorstFrameFPS=" + worstFrameFps.ToString() + " WorstTimeslice=" + m_WorstTimeslice.ToString() + " AvgFPS=" + avgFps.ToString() + " ActivePlayers=" + players.ToString();
                EoH_LiveAdvisorLogger.Log("SERVER_FRAME_HITCH_CRITICAL", hitchCriticalMessage, "critical", "EoH_Performance");
            }
            else if (m_WorstTimeslice >= EoH_LiveAdvisorLogger.m_Config.FrameHitchWarningTimeslice)
            {
                m_LastPerformanceWarningTime = now;
                string hitchWarningMessage = "Frame hitch detected. WorstFrameFPS=" + worstFrameFps.ToString() + " WorstTimeslice=" + m_WorstTimeslice.ToString() + " AvgFPS=" + avgFps.ToString() + " ActivePlayers=" + players.ToString();
                EoH_LiveAdvisorLogger.Log("SERVER_FRAME_HITCH", hitchWarningMessage, "warning", "EoH_Performance");
            }
        }

        ResetSampleWindow();
    }

    static void ResetSampleWindow()
    {
        m_SampleTimer = 0;
        m_AccumulatedTime = 0;
        m_FrameCount = 0;
        m_WorstTimeslice = 0;
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
