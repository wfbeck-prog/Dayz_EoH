class EoH_SCB_Config
{
    bool EnableDebug;
    int ContainerSize;
    bool PersistOnDeath;
    int RestoreDelayMs;

    // Tier flags (container size unlocks)
    bool Tier2x2;
    bool Tier3x3;
    bool Tier4x4;

    // Level flags (skill requirements)
    bool Level2x2;
    bool Level3x3;
    bool Level4x4;

    void EoH_SCB_Config()
    {
        EnableDebug = false;
        ContainerSize = 4;
        PersistOnDeath = true;
        RestoreDelayMs = 5000;

        // Default unlocks
        Tier2x2 = true;
        Tier3x3 = true;
        Tier4x4 = true;

        // Default level permissions
        Level2x2 = true;
        Level3x3 = true;
        Level4x4 = true;
    }
};
