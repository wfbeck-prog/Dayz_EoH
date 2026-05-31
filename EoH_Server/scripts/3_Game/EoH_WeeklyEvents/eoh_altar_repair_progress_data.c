class EoH_AltarRepairProgressData
{
    float Progress01;
    int ProgressPercent;
    int MembersInZone;
    float MaintainRadius;
    string Title;
    string Status;
    bool Complete;
    bool Failed;

    void EoH_AltarRepairProgressData()
    {
        Progress01 = 0.0;
        ProgressPercent = 0;
        MembersInZone = 0;
        MaintainRadius = 75.0;
        Title = "ALTAR RELAY REPAIR";
        Status = "Signal tether active.";
        Complete = false;
        Failed = false;
    }
}
