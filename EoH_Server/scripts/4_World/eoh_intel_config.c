class EoH_IntelConfig
{
    int RevealDurationSeconds;

    int CooldownSeconds;
    int MaxUsesPerLife;

    bool RequireItem;
    bool AllowPlayerTracking;

    void EoH_IntelConfig()
    {
        RevealDurationSeconds = 300;

        CooldownSeconds = 180;
        MaxUsesPerLife = 3;

        RequireItem = true;
        AllowPlayerTracking = true;
    }
};
