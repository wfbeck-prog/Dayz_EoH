class EoH_LiveAdvisorConfig
{
    int ConfigVersion = 1;
    bool Enabled = true;
    bool AdvisorOnly = true;
    string ServerName = "Echoes of Humanity Hardcore";
    int HeartbeatSeconds = 300;
    bool LogPlayerCounts = true;
    bool LogDebugToRPT = true;

    void Defaults()
    {
        ConfigVersion = 1;
        Enabled = true;
        AdvisorOnly = true;
        ServerName = "Echoes of Humanity Hardcore";
        HeartbeatSeconds = 300;
        LogPlayerCounts = true;
        LogDebugToRPT = true;
    }
}
