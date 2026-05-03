class EoH_SCB_Config
{
    bool EnableDebug;
    int ContainerSize;
    bool PersistOnDeath;
    int RestoreDelayMs;
    bool Level4x4;

    void EoH_SCB_Config()
    {
        EnableDebug = false;
        ContainerSize = 4;
        PersistOnDeath = true;
        RestoreDelayMs = 5000;
        Level4x4 = true;
    }
};
