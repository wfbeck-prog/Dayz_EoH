class EoH_SCB_Config
{
    // TODO: Replace with your actual A6 container classnames.
    // Keep max secure size at 4x4 for EoH balance.
    string Tier2x2 = "A6_SecureContainer_Small";
    string Tier3x3 = "A6_SecureContainer_Medium";
    string Tier4x4 = "A6_SecureContainer_Large";

    int Level2x2 = 3;
    int Level3x3 = 5;
    int Level4x4 = 8;

    string SaveFolder = "$profile:EoH/SecureContainers/";
}

class EoH_SCB
{
    static ref EoH_SCB_Config Config;

    static void Init()
    {
        if (!Config)
            Config = new EoH_SCB_Config();
    }

    static string GetAllowedContainerClass(PlayerBase player)
    {
        Init();

        int level = GetTerjeSurvivalLevel(player);

        if (level >= Config.Level4x4)
            return Config.Tier4x4;

        if (level >= Config.Level3x3)
            return Config.Tier3x3;

        if (level >= Config.Level2x2)
            return Config.Tier2x2;

        return string.Empty;
    }

    // IMPORTANT:
    // Replace this placeholder with the actual Terje skill getter from your installed Terje version.
    static int GetTerjeSurvivalLevel(PlayerBase player)
    {
        return 0;
    }
}
