class EoH_LiveAdvisorConfig
{
    int ConfigVersion = 2;
    bool Enabled = true;
    bool AdvisorOnly = true;
    string ServerName = "Echoes of Humanity Hardcore";
    int HeartbeatSeconds = 300;
    bool LogPlayerCounts = true;
    bool LogDebugToRPT = true;
    bool WriteCopyPasteReport = true;
    int MaxReportLines = 75;

    void Defaults()
    {
        ConfigVersion = 2;
        Enabled = true;
        AdvisorOnly = true;
        ServerName = "Echoes of Humanity Hardcore";
        HeartbeatSeconds = 300;
        LogPlayerCounts = true;
        LogDebugToRPT = true;
        WriteCopyPasteReport = true;
        MaxReportLines = 75;
    }
}
