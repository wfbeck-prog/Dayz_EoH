class EoH_EventRewardCrate
{
    Object CrateObject;
    bool Locked;
    int UnlockTime;

    void EoH_EventRewardCrate()
    {
        CrateObject = null;
        Locked = true;
        UnlockTime = 0;
    }
}
