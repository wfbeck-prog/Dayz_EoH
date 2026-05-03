class EoH_SCB_Config
{
    bool EnableDebug;
    int ContainerSize;
    bool PersistOnDeath;

    void EoH_SCB_Config()
    {
        EnableDebug = false;
        ContainerSize = 4;
        PersistOnDeath = true;
    }
};
