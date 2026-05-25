class EoH_EventRewardCrate
{
    Object CrateObject;
    bool Locked;
    bool Spawned;
    int UnlockTime;
    string CrateType;
    vector Position;

    void EoH_EventRewardCrate()
    {
        CrateObject = null;
        Locked = true;
        Spawned = false;
        UnlockTime = 0;
        CrateType = "SeaChest";
        Position = "0 0 0";
    }

    void SetRuntime(vector pos, int delayMinutes, int lootTier)
    {
        Position = pos;
        UnlockTime = GetGame().GetTime() + (delayMinutes * 60 * 1000);
        Locked = true;
        Spawned = false;

        if (lootTier >= 4)
            CrateType = "SeaChest";
        else
            CrateType = "WoodenCrate";
    }

    bool ShouldUnlock()
    {
        if (!Locked)
            return false;

        if (UnlockTime <= 0)
            return false;

        return GetGame().GetTime() >= UnlockTime;
    }

    void MarkSpawned(Object obj)
    {
        CrateObject = obj;
        Spawned = (CrateObject != null);
    }

    void MarkUnlocked()
    {
        Locked = false;
    }

    void Cleanup()
    {
        if (CrateObject)
            GetGame().ObjectDelete(CrateObject);

        CrateObject = null;
        Spawned = false;
        Locked = true;
    }
}
