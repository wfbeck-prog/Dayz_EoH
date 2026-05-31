class EoH_ObjectiveWaveData
{
    string Channel;
    string Title;
    string Status;

    int CurrentWave;
    int MaxWaves;
    int ProgressPercent;

    bool FinalWave;
    bool RewardDeployed;
    bool Complete;

    void EoH_ObjectiveWaveData()
    {
        Channel = "";
        Title = "HOSTILE RESPONSE";
        Status = "";
        CurrentWave = 0;
        MaxWaves = 0;
        ProgressPercent = 0;
        FinalWave = false;
        RewardDeployed = false;
        Complete = false;
    }

    void Normalize()
    {
        if (CurrentWave < 0)
            CurrentWave = 0;
        if (MaxWaves < 0)
            MaxWaves = 0;
        if (CurrentWave > MaxWaves && MaxWaves > 0)
            CurrentWave = MaxWaves;

        if (MaxWaves > 0)
            ProgressPercent = Math.Round((CurrentWave / MaxWaves) * 100.0);
        else
            ProgressPercent = 0;

        if (ProgressPercent < 0)
            ProgressPercent = 0;
        if (ProgressPercent > 100)
            ProgressPercent = 100;

        if (Title == "")
            Title = "HOSTILE RESPONSE";
    }
}
