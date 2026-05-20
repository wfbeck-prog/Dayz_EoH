class EoH_TownRewardItemConfig
{
    string ClassName;
    int Min;
    int Max;
    float Chance;

    void EoH_TownRewardItemConfig()
    {
        ClassName = "";
        Min = 1;
        Max = 1;
        Chance = 1.0;
    }
};

class EoH_TownRewardTierConfig
{
    int Tier;
    string Container;
    string SmokeType;
    int LifetimeSeconds;
    ref array<ref EoH_TownRewardItemConfig> Loot;

    void EoH_TownRewardTierConfig()
    {
        Tier = 1;
        Container = "AmmoBox";
        SmokeType = "M18SmokeGrenade_Red";
        LifetimeSeconds = 3600;
        Loot = new array<ref EoH_TownRewardItemConfig>();
    }
};

class EoH_TownRewardTownConfig
{
    string TownName;
    int Tier;

    void EoH_TownRewardTownConfig()
    {
        TownName = "";
        Tier = 1;
    }
};

class EoH_TownRewardConfig
{
    int ConfigVersion;
    ref array<ref EoH_TownRewardTownConfig> Towns;
    ref array<ref EoH_TownRewardTierConfig> Tiers;

    void EoH_TownRewardConfig()
    {
        ConfigVersion = 1;
        Towns = new array<ref EoH_TownRewardTownConfig>();
        Tiers = new array<ref EoH_TownRewardTierConfig>();
    }
};
