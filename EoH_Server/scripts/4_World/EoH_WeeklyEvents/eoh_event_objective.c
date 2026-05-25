class EoH_EventObjective
{
    string Id;
    string DisplayName;
    vector Position;
    string ObjectiveType;
    string SpawnObject;
    int RecommendedPlayers;
    int LootTier;
    bool EnableAIBandits;
    bool EnableInfected;
    bool EnableMarker;
    bool EnableSmoke;

    void EoH_EventObjective()
    {
        Id = "";
        DisplayName = "Unknown Objective";
        Position = "0 0 0";
        ObjectiveType = "signal";
        SpawnObject = "";
        RecommendedPlayers = 4;
        LootTier = 1;
        EnableAIBandits = true;
        EnableInfected = true;
        EnableMarker = true;
        EnableSmoke = true;
    }
}

class EoH_EventObjectiveRuntime
{
    ref EoH_EventObjective Config;
    Object SpawnedObject;
    ref EoH_EventRewardCrate RewardCrate;
    bool Active;
    bool RevealedByIntel;
    bool RewardUnlocked;
    bool Completed;
    int StartTime;
    int LastTickTime;
    int CurrentWave;

    void EoH_EventObjectiveRuntime(EoH_EventObjective cfg)
    {
        Config = cfg;
        SpawnedObject = null;
        RewardCrate = null;
        Active = false;
        RevealedByIntel = false;
        RewardUnlocked = false;
        Completed = false;
        StartTime = 0;
        LastTickTime = 0;
        CurrentWave = -1;
    }
}
