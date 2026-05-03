class EoH_SCB_Config
{
    bool EnableDebug;
    int ContainerSize;
    bool PersistOnDeath;
    int RestoreDelayMs;

    void EoH_SCB_Config()
    {
        EnableDebug = false;
        ContainerSize = 4;
        PersistOnDeath = true;
        RestoreDelayMs = 5000;
    }
};
