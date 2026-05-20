class EoH_TownAILoadoutSet
{
    ref array<string> PatrolLoadouts;
    ref array<string> CampLoadouts;

    void EoH_TownAILoadoutSet()
    {
        PatrolLoadouts = new array<string>();
        CampLoadouts = new array<string>();
    }
};

class EoH_TownAITierConfig
{
    int Tier;
    bool Enabled;
    int MinPatrols;
    int MaxPatrols;
    int MinCamps;
    int MaxCamps;
    int MinAIPerPatrol;
    int MaxAIPerPatrol;
    int MinAIPerCamp;
    int MaxAIPerCamp;
    float SpawnRadiusMin;
    float SpawnRadiusMax;
    float DespawnRadius;
    ref EoH_TownAILoadoutSet Loadouts;

    void EoH_TownAITierConfig()
    {
        Tier = 1;
        Enabled = true;
        MinPatrols = 0;
        MaxPatrols = 1;
        MinCamps = 0;
        MaxCamps = 1;
        MinAIPerPatrol = 1;
        MaxAIPerPatrol = 2;
        MinAIPerCamp = 2;
        MaxAIPerCamp = 3;
        SpawnRadiusMin = 80.0;
        SpawnRadiusMax = 220.0;
        DespawnRadius = 650.0;
        Loadouts = new EoH_TownAILoadoutSet();
    }
};

class EoH_TownAITownConfig
{
    string TownName;
    int Tier;
    bool Enabled;
    bool SpawnWhenUnclaimed;
    bool SpawnWhenOwned;
    bool SpawnWhenContested;
    bool SpawnAfterCapture;

    void EoH_TownAITownConfig()
    {
        TownName = "";
        Tier = 1;
        Enabled = true;
        SpawnWhenUnclaimed = true;
        SpawnWhenOwned = true;
        SpawnWhenContested = true;
        SpawnAfterCapture = true;
    }
};

class EoH_TownAIConfig
{
    int ConfigVersion;
    bool Enabled;
    int TickSeconds;
    int MaxActiveTowns;
    int RespawnCooldownSeconds;
    ref array<ref EoH_TownAITownConfig> Towns;
    ref array<ref EoH_TownAITierConfig> Tiers;

    void EoH_TownAIConfig()
    {
        ConfigVersion = 1;
        Enabled = false;
        TickSeconds = 60;
        MaxActiveTowns = 4;
        RespawnCooldownSeconds = 1800;
        Towns = new array<ref EoH_TownAITownConfig>();
        Tiers = new array<ref EoH_TownAITierConfig>();
    }
};
