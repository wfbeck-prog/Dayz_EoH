class EoH_LiveAdvisorConfig
{
    int ConfigVersion = 5;
    bool Enabled = true;
    bool AdvisorOnly = true;
    string ServerName = "Echoes of Humanity Hardcore";
    int HeartbeatSeconds = 300;
    bool LogPlayerCounts = true;
    bool LogDebugToRPT = true;
    bool WriteCopyPasteReport = true;
    int MaxReportLines = 75;
    bool EnableBunkerPanelObserver = false;

    bool EnablePerformanceMonitor = true;
    bool LogPerformanceSamples = true;
    int PerformanceSampleSeconds = 30;
    float LowFpsWarningThreshold = 10.0;
    float CriticalFpsThreshold = 5.0;
    int PerformanceWarningCooldownSeconds = 300;

    bool EnableActivityCorrelation = true;
    int ActivityReportCooldownSeconds = 60;

    void Defaults()
    {
        ConfigVersion = 5;
        Enabled = true;
        AdvisorOnly = true;
        ServerName = "Echoes of Humanity Hardcore";
        HeartbeatSeconds = 300;
        LogPlayerCounts = true;
        LogDebugToRPT = true;
        WriteCopyPasteReport = true;
        MaxReportLines = 75;
        EnableBunkerPanelObserver = false;

        EnablePerformanceMonitor = true;
        LogPerformanceSamples = true;
        PerformanceSampleSeconds = 30;
        LowFpsWarningThreshold = 10.0;
        CriticalFpsThreshold = 5.0;
        PerformanceWarningCooldownSeconds = 300;

        EnableActivityCorrelation = true;
        ActivityReportCooldownSeconds = 60;
    }
}
