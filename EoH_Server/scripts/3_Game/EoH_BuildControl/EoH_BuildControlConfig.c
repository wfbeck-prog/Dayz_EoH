static const string EOH_BUILD_CONTROL_PROFILE_DIR = "$profile:EoH_Server";
static const string EOH_BUILD_CONTROL_CONFIG_PATH = "$profile:EoH_Server/BuildControl.json";

class EoH_BuildControlRule
{
    string ClassName;
    ref array<string> MatchAlso;

    bool BlockPlacement;
    bool RequiresTerritory;
    bool AllowOutsideTerritory;
    bool IgnoreTerritoryCheck;

    int MaxPlacedPerPlayer;
    float CountRadius;

    string DenyMessage;

    void EoH_BuildControlRule()
    {
        ClassName = "";
        MatchAlso = new array<string>;
        BlockPlacement = false;
        RequiresTerritory = false;
        AllowOutsideTerritory = false;
        IgnoreTerritoryCheck = false;
        MaxPlacedPerPlayer = -1;
        CountRadius = 1200.0;
        DenyMessage = "You cannot place that here.";
    }

    bool Matches(string typeName)
    {
        if (typeName == ClassName)
            return true;

        foreach (string also : MatchAlso)
        {
            if (also == typeName)
                return true;
        }

        return false;
    }
};

class EoH_BuildControlConfig
{
    bool Enabled;
    bool RequireTerritoryForBuilding;
    bool AllowFlagKitOutsideTerritory;
    bool EnforcePerPlayerLimits;

    float TerritoryRadiusMeters;
    float TerritoryConflictRadiusMeters;

    ref array<string> AdminSteamIds;
    ref array<string> TerritoryAnchorClasses;
    ref array<string> BuildWhitelist;
    ref array<string> BuildBlacklist;
    ref array<ref EoH_BuildControlRule> Rules;

    string TerritoryRequiredMessage;
    string TerritoryConflictMessage;
    string BlacklistMessage;
    string LimitReachedMessage;

    void EoH_BuildControlConfig()
    {
        Enabled = true;
        RequireTerritoryForBuilding = true;
        AllowFlagKitOutsideTerritory = true;
        EnforcePerPlayerLimits = true;

        TerritoryRadiusMeters = 80.0;
        TerritoryConflictRadiusMeters = 160.0;

        AdminSteamIds = new array<string>;
        TerritoryAnchorClasses = new array<string>;
        BuildWhitelist = new array<string>;
        BuildBlacklist = new array<string>;
        Rules = new array<ref EoH_BuildControlRule>;

        TerritoryRequiredMessage = "You must be inside a territory to build here.";
        TerritoryConflictMessage = "Another territory is too close.";
        BlacklistMessage = "That item is restricted on Echoes of Humanity.";
        LimitReachedMessage = "You have reached the placement limit for that item.";
    }

    void Defaults()
    {
        TerritoryAnchorClasses.Clear();
        TerritoryAnchorClasses.Insert("TerritoryFlag");

        BuildWhitelist.Clear();
        BuildWhitelist.Insert("TerritoryFlagKit");
        BuildWhitelist.Insert("Flag_Base");

        BuildBlacklist.Clear();
        BuildBlacklist.Insert("BarrelHoles_ColorBase");
        BuildBlacklist.Insert("FireplaceBase");

        Rules.Clear();

        EoH_BuildControlRule flagRule = new EoH_BuildControlRule();
        flagRule.ClassName = "TerritoryFlagKit";
        flagRule.IgnoreTerritoryCheck = true;
        flagRule.MaxPlacedPerPlayer = 1;
        flagRule.CountRadius = 2500.0;
        flagRule.DenyMessage = "You already control a territory flag.";
        Rules.Insert(flagRule);

        EoH_BuildControlRule tentRule = new EoH_BuildControlRule();
        tentRule.ClassName = "TentBase";
        tentRule.RequiresTerritory = true;
        tentRule.MaxPlacedPerPlayer = 3;
        tentRule.CountRadius = 1200.0;
        tentRule.DenyMessage = "Tent placement is limited by EoH territory rules.";
        Rules.Insert(tentRule);
    }

    static EoH_BuildControlConfig Load()
    {
        EoH_BuildControlConfig cfg = new EoH_BuildControlConfig();

        if (!FileExist(EOH_BUILD_CONTROL_PROFILE_DIR))
            MakeDirectory(EOH_BUILD_CONTROL_PROFILE_DIR);

        if (FileExist(EOH_BUILD_CONTROL_CONFIG_PATH))
        {
            JsonFileLoader<EoH_BuildControlConfig>.JsonLoadFile(EOH_BUILD_CONTROL_CONFIG_PATH, cfg);
        }
        else
        {
            cfg.Defaults();
            JsonFileLoader<EoH_BuildControlConfig>.JsonSaveFile(EOH_BUILD_CONTROL_CONFIG_PATH, cfg);
        }

        return cfg;
    }

    EoH_BuildControlRule FindRule(string typeName)
    {
        foreach (EoH_BuildControlRule rule : Rules)
        {
            if (rule && rule.Matches(typeName))
                return rule;
        }

        return null;
    }

    bool IsAdmin(string steamId)
    {
        if (steamId == "")
            return false;

        return AdminSteamIds.Find(steamId) >= 0;
    }
};

static ref EoH_BuildControlConfig g_EoH_BuildControlConfig;

EoH_BuildControlConfig GetEoHBuildControlConfig()
{
    if (!g_EoH_BuildControlConfig)
        g_EoH_BuildControlConfig = EoH_BuildControlConfig.Load();

    return g_EoH_BuildControlConfig;
}
