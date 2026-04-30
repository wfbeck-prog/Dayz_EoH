class EoH_AITownSpawnPoint
{
    ref array<float> Position;
    string Role;

    void EoH_AITownSpawnPoint()
    {
        Position = new array<float>();
        Role = "Patrol";
    }

    vector GetPosition()
    {
        if (!Position || Position.Count() < 3)
            return vector.Zero;

        return Vector(Position.Get(0), Position.Get(1), Position.Get(2));
    }
};

class EoH_AITownConfig
{
    string TownName;
    int Tier;
    string Behavior;
    int MinAI;
    int MaxAI;
    int RespawnSeconds;
    ref array<ref EoH_AITownSpawnPoint> SpawnPoints;

    void EoH_AITownConfig()
    {
        TownName = "";
        Tier = 1;
        Behavior = "Defend";
        MinAI = 2;
        MaxAI = 4;
        RespawnSeconds = 1800;
        SpawnPoints = new array<ref EoH_AITownSpawnPoint>();
    }
};

class EoH_AITownSystemConfig
{
    int ConfigVersion;
    bool Enabled;
    bool SpawnOnCapture;
    bool DespawnWhenUnclaimed;
    ref array<ref EoH_AITownConfig> Towns;

    void EoH_AITownSystemConfig()
    {
        ConfigVersion = 1;
        Enabled = true;
        SpawnOnCapture = true;
        DespawnWhenUnclaimed = true;
        Towns = new array<ref EoH_AITownConfig>();
    }
};
