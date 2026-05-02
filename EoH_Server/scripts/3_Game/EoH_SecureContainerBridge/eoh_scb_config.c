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
