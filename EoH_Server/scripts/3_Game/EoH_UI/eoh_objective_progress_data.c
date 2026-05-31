class EoH_ObjectiveProgressData
{
    string Channel;
    string Title;
    string Status;
    string LeftLabel;
    string RightLabel;

    float Progress01;
    int ProgressPercent;

    bool Complete;
    bool Failed;
    bool Contested;

    void EoH_ObjectiveProgressData()
    {
        Channel = "";
        Title = "OBJECTIVE";
        Status = "";
        LeftLabel = "";
        RightLabel = "";
        Progress01 = 0.0;
        ProgressPercent = 0;
        Complete = false;
        Failed = false;
        Contested = false;
    }

    void Normalize()
    {
        if (Progress01 < 0.0)
            Progress01 = 0.0;
        if (Progress01 > 1.0)
            Progress01 = 1.0;

        ProgressPercent = Math.Round(Progress01 * 100.0);
        if (ProgressPercent < 0)
            ProgressPercent = 0;
        if (ProgressPercent > 100)
            ProgressPercent = 100;

        if (Title == "")
            Title = "OBJECTIVE";
    }
}
