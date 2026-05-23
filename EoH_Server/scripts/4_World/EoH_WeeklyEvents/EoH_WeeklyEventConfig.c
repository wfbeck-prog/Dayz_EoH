class EoH_WeeklyEventConfig
{
    int ConfigVersion;
    bool Enabled;
    bool EnableDebug;
    int TickSeconds;
    int RelayCollapseEnabled;
    int PurgeNightEnabled;
    int RelayWarningMinutes;
    int RelayActiveMinutes;
    int RelayCooldownHours;
    float RelayEventRadius;

    void EoH_WeeklyEventConfig()
    {
        ConfigVersion = 1;
        Enabled = true;
        EnableDebug = true;
        TickSeconds = 60;
        RelayCollapseEnabled = 1;
        PurgeNightEnabled = 0;
        RelayWarningMinutes = 10;
        RelayActiveMinutes = 60;
        RelayCooldownHours = 168;
        RelayEventRadius = 500.0;
    }

    static EoH_WeeklyEventConfig Load()
    {
        string dir = "$profile:EoH_Server";
        string path = dir + "/WeeklyEvents.json";

        if (!FileExist(dir))
            MakeDirectory(dir);

        EoH_WeeklyEventConfig cfg;
        if (FileExist(path))
        {
            JsonFileLoader<EoH_WeeklyEventConfig>.JsonLoadFile(path, cfg);
            if (cfg)
                return cfg;
        }

        cfg = new EoH_WeeklyEventConfig();
        JsonFileLoader<EoH_WeeklyEventConfig>.JsonSaveFile(path, cfg);
        return cfg;
    }
}
