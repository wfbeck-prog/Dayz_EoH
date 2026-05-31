class EoH_AdvisorRecommendationManager
{
    protected static ref EoH_AdvisorRecommendationManager s_Instance;
    protected ref EoH_AdvisorRecommendationConfig m_Config;
    protected float m_LastRecommendationTime;
    protected float m_LastHeartbeatTime;
    protected int m_RepeatedHitchCount;

    protected const string CONFIG_DIR = "$profile:EoH";
    protected const string CONFIG_PATH = "$profile:EoH/AdvisorRecommendations.json";
    protected const float HEARTBEAT_SECONDS = 300.0;

    void EoH_AdvisorRecommendationManager()
    {
        m_LastRecommendationTime = 0;
        m_LastHeartbeatTime = 0;
        m_RepeatedHitchCount = 0;
        LoadConfig();
    }

    static EoH_AdvisorRecommendationManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_AdvisorRecommendationManager();

        return s_Instance;
    }

    void LoadConfig()
    {
        EnsureConfigDir();
        m_Config = new EoH_AdvisorRecommendationConfig();

        if (!FileExist(CONFIG_PATH))
        {
            m_Config.Defaults();
            JsonFileLoader<EoH_AdvisorRecommendationConfig>.JsonSaveFile(CONFIG_PATH, m_Config);
            EoH_LiveAdvisorActivity.LogActivity("advisor", "created_default_config path=" + CONFIG_PATH);
        }
        else
        {
            JsonFileLoader<EoH_AdvisorRecommendationConfig>.JsonLoadFile(CONFIG_PATH, m_Config);
            NormalizeConfig();
            EoH_LiveAdvisorActivity.LogActivity("advisor", "loaded_config enabled=" + m_Config.Enabled.ToString() + " warnOnly=" + m_Config.WarnOnly.ToString());
        }

        WriteStartupState();
    }

    void NormalizeConfig()
    {
        if (!m_Config)
            return;

        if (m_Config.EmptyServerExtremeHitchTimeslice <= 0)
            m_Config.EmptyServerExtremeHitchTimeslice = 0.5;

        if (m_Config.RepeatedHitchTimeslice <= 0)
            m_Config.RepeatedHitchTimeslice = 0.25;

        if (m_Config.RepeatedHitchCountThreshold <= 0)
            m_Config.RepeatedHitchCountThreshold = 3;

        if (m_Config.RecommendationCooldownSeconds <= 0)
            m_Config.RecommendationCooldownSeconds = 300;

        if (m_Config.TownAIActiveWarningThreshold <= 0)
            m_Config.TownAIActiveWarningThreshold = 6;

        if (m_Config.SchedulerFastIntervalWarningMs <= 0)
            m_Config.SchedulerFastIntervalWarningMs = 30000;
    }

    void EnsureConfigDir()
    {
        if (!FileExist(CONFIG_DIR))
            MakeDirectory(CONFIG_DIR);
    }

    bool IsEnabled()
    {
        return m_Config && m_Config.Enabled;
    }

    void Tick()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!IsEnabled())
            return;

        float now = GetGame().GetTime() / 1000.0;
        WriteHeartbeat(now);
    }

    void WriteStartupState()
    {
        if (!m_Config)
            return;

        EoH_LiveAdvisorActivity.LogActivity("advisor", "recommendation_engine_ready performance=" + m_Config.EnablePerformanceRecommendations.ToString() + " koth=" + m_Config.EnableKothRecommendations.ToString() + " townAI=" + m_Config.EnableTownAIRecommendations.ToString() + " scheduler=" + m_Config.EnableSchedulerRecommendations.ToString() + " ignoreEmptyServerHitches=" + m_Config.IgnoreEmptyServerHitches.ToString() + " emptyServerExtreme=" + m_Config.EmptyServerExtremeHitchTimeslice.ToString());
    }

    void WriteHeartbeat(float now)
    {
        if (!m_Config)
            return;

        if (m_LastHeartbeatTime > 0 && now - m_LastHeartbeatTime < HEARTBEAT_SECONDS)
            return;

        m_LastHeartbeatTime = now;
        EoH_LiveAdvisorActivity.LogActivity("advisor", "heartbeat enabled=" + m_Config.Enabled.ToString() + " repeatedHitches=" + m_RepeatedHitchCount.ToString() + " cooldownSeconds=" + m_Config.RecommendationCooldownSeconds.ToString() + " ignoreEmpty=" + m_Config.IgnoreEmptyServerHitches.ToString());
    }

    void ObservePerformanceSample(float worstTimeslice, float avgFps, int players)
    {
        if (!IsEnabled() || !m_Config.EnablePerformanceRecommendations)
            return;

        if (ShouldIgnoreEmptyServerHitch(worstTimeslice, players))
        {
            if (m_RepeatedHitchCount > 0)
                m_RepeatedHitchCount = 0;
            return;
        }

        if (worstTimeslice >= m_Config.RepeatedHitchTimeslice)
        {
            m_RepeatedHitchCount++;
        }
        else
        {
            if (m_RepeatedHitchCount > 0)
                m_RepeatedHitchCount--;
        }

        if (m_RepeatedHitchCount >= m_Config.RepeatedHitchCountThreshold)
        {
            Recommend("performance", "Repeated frame hitches detected. WorstTimeslice=" + worstTimeslice.ToString() + " AvgFPS=" + avgFps.ToString() + " Players=" + players.ToString() + ". Review recent KOTH, AI, cleanup, or scheduler activity around the same timestamp.");
            m_RepeatedHitchCount = 0;
        }
    }

    bool ShouldIgnoreEmptyServerHitch(float worstTimeslice, int players)
    {
        if (!m_Config)
            return false;

        if (!m_Config.IgnoreEmptyServerHitches)
            return false;

        if (players > 0)
            return false;

        if (worstTimeslice >= m_Config.EmptyServerExtremeHitchTimeslice)
            return false;

        return true;
    }

    void ObserveTownAI(int activeTowns, int monitorElapsedMs)
    {
        if (!IsEnabled() || !m_Config.EnableTownAIRecommendations)
            return;

        if (activeTowns >= m_Config.TownAIActiveWarningThreshold)
        {
            Recommend("town_ai", "TownAI active town count is high. ActiveTowns=" + activeTowns.ToString() + " MonitorElapsedMs=" + monitorElapsedMs.ToString() + ". Consider increasing scan intervals or reducing simultaneous active town radius if FPS drops.");
        }
    }

    void ObserveScheduler(string taskName, int elapsedMs, int intervalMs)
    {
        if (!IsEnabled() || !m_Config.EnableSchedulerRecommendations)
            return;

        if (intervalMs > 0 && intervalMs <= m_Config.SchedulerFastIntervalWarningMs)
        {
            Recommend("scheduler", "Scheduler task is running frequently. Task=" + taskName + " IntervalMs=" + intervalMs.ToString() + " ElapsedMs=" + elapsedMs.ToString() + ". For live server use, consider 60000ms or higher unless this is intentional.");
        }
    }

    void ObserveKothRisk(string zoneName, string nearestTown, float dist, float relayDist)
    {
        if (!IsEnabled() || !m_Config.EnableKothRecommendations)
            return;

        if (dist <= 50 || relayDist <= 150)
        {
            Recommend("koth", "Player is very close to a KOTH/EoH overlap risk area. Zone=" + zoneName + " Town=" + nearestTown + " Dist=" + dist.ToString() + " RelayDist=" + relayDist.ToString() + ". If crashes or FPS drops occur, test moving KOTH away from town relay markers or enable KOTH Safe Mode.");
        }
    }

    void Recommend(string category, string message)
    {
        float now = GetGame().GetTime() / 1000.0;
        if (m_LastRecommendationTime > 0 && now - m_LastRecommendationTime < m_Config.RecommendationCooldownSeconds)
            return;

        m_LastRecommendationTime = now;
        EoH_LiveAdvisorLogger.Log("ADVISOR_RECOMMENDATION", "Category=" + category + " " + message, "warning", "EoH_Advisor");
    }
}
