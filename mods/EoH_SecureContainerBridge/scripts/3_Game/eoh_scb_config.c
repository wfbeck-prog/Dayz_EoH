class EoH_SCB_Config
{
    // TODO: Replace with your actual A6 container classnames
    string Tier2x2 = "A6_SecureContainer_Small";
    string Tier3x3 = "A6_SecureContainer_Medium";
    string Tier4x4 = "A6_SecureContainer_Large";

    int Level2x2 = 3;
    int Level3x3 = 5;
    int Level4x4 = 8;
}

class EoH_SCB
{
    static ref EoH_SCB_Config Config;

    static void Init()
    {
        if (!Config)
            Config = new EoH_SCB_Config();
    }

    // IMPORTANT: Replace this with actual Terje API call
    static int GetTerjeSurvivalLevel(PlayerBase player)
    {
        return 0; // placeholder
    }
}
