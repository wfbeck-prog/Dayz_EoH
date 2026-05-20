class EoH_TownAIActiveTown
{
    string TownName;
    int Tier;
    int LastSpawnTime;
    ref array<Object> SpawnedObjects;

    void EoH_TownAIActiveTown()
    {
        TownName = "";
        Tier = 1;
        LastSpawnTime = 0;
        SpawnedObjects = new array<Object>();
    }
};
