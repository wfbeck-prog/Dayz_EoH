class EoH_SCB_Config
{
    // A6 Secure Containers classnames found in your uploaded A6 mod.
    // Recommended EoH progression keeps this intentionally small.
    string Tier2x2 = "A6_SecureContainer_Alpha";
    string Tier3x3 = "A6_SecureContainer_Beta";
    string Tier4x4 = "A6_SecureContainer_Gamma";

    // Optional larger A6 cases exist but are not used by default for EoH balance:
    // A6_SecureContainer_Epsilon
    // A6_SecureContainer_Kappa

    int Level2x2 = 3;
    int Level3x3 = 5;
    int Level4x4 = 8;

    // Delay gives Terje StartScreen enough time to finish applying spawn point/loadout.
    int RestoreDelayMs = 15000;
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

    static bool IsA6SecureContainer(EntityAI item)
    {
        return item && item.IsKindOf("A6_SecureContainer_Base");
    }

    static bool IsAllowedA6Class(string className)
    {
        Init();
        return className == Config.Tier2x2 || className == Config.Tier3x3 || className == Config.Tier4x4;
    }

    static int GetTerjeSurvivalLevel(PlayerBase player)
    {
        if (!player || !player.GetTerjeSkills())
            return 0;

        // Terje Skills survival skill id is "surv".
        return player.GetTerjeSkills().GetSkillLevel("surv");
    }
}
