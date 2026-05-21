class EoH_TownAIActiveTown
{
    string TownName;
    int Tier;
    string OwnerGroupID;
    string OwnerGroupName;
    int LastSpawnTime;
    ref array<Object> SpawnedObjects;

    void EoH_TownAIActiveTown()
    {
        TownName = "";
        Tier = 1;
        OwnerGroupID = "";
        OwnerGroupName = "Unclaimed";
        LastSpawnTime = 0;
        SpawnedObjects = new array<Object>();
    }
};
