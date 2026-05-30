class EoH_WeeklyEventConfig
{
    int ConfigVersion = 1;
    bool Enabled = true;

    float AltarRepairDurationSeconds = 10.0;
    float AltarRepairProximityRadius = 5.0;
    int AltarRepairWatcherIntervalMs = 5000;

    void Defaults()
    {
        ConfigVersion = 1;
        Enabled = true;

        AltarRepairDurationSeconds = 10.0;
        AltarRepairProximityRadius = 5.0;
        AltarRepairWatcherIntervalMs = 5000;
    }
}
