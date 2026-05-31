class EoH_AdvisorRecommendationConfig
{
    int ConfigVersion = 2;
    bool Enabled = true;
    bool WarnOnly = true;

    bool EnableKothRecommendations = true;
    bool EnablePerformanceRecommendations = true;
    bool EnableTownAIRecommendations = true;
    bool EnableSchedulerRecommendations = true;

    bool IgnoreEmptyServerHitches = true;
    float EmptyServerExtremeHitchTimeslice = 0.5;

    float RepeatedHitchTimeslice = 0.25;
    int RepeatedHitchCountThreshold = 3;
    int RecommendationCooldownSeconds = 300;

    int TownAIActiveWarningThreshold = 6;
    int SchedulerFastIntervalWarningMs = 30000;

    void Defaults()
    {
        ConfigVersion = 2;
        Enabled = true;
        WarnOnly = true;

        EnableKothRecommendations = true;
        EnablePerformanceRecommendations = true;
        EnableTownAIRecommendations = true;
        EnableSchedulerRecommendations = true;

        IgnoreEmptyServerHitches = true;
        EmptyServerExtremeHitchTimeslice = 0.5;

        RepeatedHitchTimeslice = 0.25;
        RepeatedHitchCountThreshold = 3;
        RecommendationCooldownSeconds = 300;

        TownAIActiveWarningThreshold = 6;
        SchedulerFastIntervalWarningMs = 30000;
    }
}
