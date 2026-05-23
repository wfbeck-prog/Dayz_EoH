class EoH_WeeklyRelayTowerLocation
{
    string Id;
    string DisplayName;
    vector Position;
    float Radius;

    void EoH_WeeklyRelayTowerLocation()
    {
        Id = "";
        DisplayName = "";
        Position = "0 0 0".ToVector();
        Radius = 500.0;
    }
}

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
    ref array<ref EoH_WeeklyRelayTowerLocation> RelayTowers;

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
        RelayTowers = new array<ref EoH_WeeklyRelayTowerLocation>();
        AddDefaultRelayTowers();
    }

    void AddDefaultRelayTowers()
    {
        EoH_WeeklyRelayTowerLocation tower = new EoH_WeeklyRelayTowerLocation();
        tower.Id = "relay_tower_001";
        tower.DisplayName = "Relay Tower 001";
        tower.Position = "8132.950684 492.125763 9093.746094".ToVector();
        tower.Radius = RelayEventRadius;
        RelayTowers.Insert(tower);
    }

    void EnsureDefaults()
    {
        if (!RelayTowers)
            RelayTowers = new array<ref EoH_WeeklyRelayTowerLocation>();

        if (RelayTowers.Count() == 0)
            AddDefaultRelayTowers();
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
            {
                cfg.EnsureDefaults();
                JsonFileLoader<EoH_WeeklyEventConfig>.JsonSaveFile(path, cfg);
                return cfg;
            }
        }

        cfg = new EoH_WeeklyEventConfig();
        JsonFileLoader<EoH_WeeklyEventConfig>.JsonSaveFile(path, cfg);
        return cfg;
    }
}
