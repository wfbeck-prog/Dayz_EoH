class EoH_AdvisorRecommendationConfig
{
    int ConfigVersion = 1;
    bool Enabled = true;
    bool WarnOnly = true;

    bool EnableKothRecommendations = true;
    bool EnablePerformanceRecommendations = true;
    bool EnableTownAIRecommendations = true;
    bool EnableSchedulerRecommendations = true;

    float RepeatedHitchTimeslice = 0.25;
    int RepeatedHitchCountThreshold = 3;
    int RecommendationCooldownSeconds = 300;

    int TownAIActiveWarningThreshold = 6;
    int SchedulerFastIntervalWarningMs = 30000;

    void Defaults()
    {
        ConfigVersion = 1;
        Enabled = true;
        WarnOnly = true;

        EnableKothRecommendations = true;
        EnablePerformanceRecommendations = true;
        EnableTownAIRecommendations = true;
        EnableSchedulerRecommendations = true;

        RepeatedHitchTimeslice = 0.25;
        RepeatedHitchCountThreshold = 3;
        RecommendationCooldownSeconds = 300;

        TownAIActiveWarningThreshold = 6;
        SchedulerFastIntervalWarningMs = 30000;
    }
}
