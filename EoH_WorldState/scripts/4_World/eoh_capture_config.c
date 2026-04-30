class EoH_CaptureTownConfig
{
    string Name;
    int Tier;
    float Radius;
    float ContestRadius;
    int CaptureTimeSeconds;
    int RequiredGroupMembers;
    ref array<float> RelayPosition;

    void EoH_CaptureTownConfig()
    {
        Name = "";
        Tier = 1;
        Radius = 200.0;
        ContestRadius = 80.0;
        CaptureTimeSeconds = 1200;
        RequiredGroupMembers = 2;
        RelayPosition = new array<float>();
    }

    vector GetRelayVector()
    {
        if (!RelayPosition || RelayPosition.Count() < 3)
            return vector.Zero;

        return Vector(RelayPosition.Get(0), RelayPosition.Get(1), RelayPosition.Get(2));
    }
};

class EoH_CaptureConfig
{
    int ConfigVersion;
    bool Enabled;
    bool RequireSameGroup;
    bool PauseWhenContested;
    bool AllowSoloCaptureFallback;
    int TickSeconds;
    ref array<ref EoH_CaptureTownConfig> Towns;

    void EoH_CaptureConfig()
    {
        ConfigVersion = 1;
        Enabled = true;
        RequireSameGroup = true;
        PauseWhenContested = true;
        AllowSoloCaptureFallback = false;
        TickSeconds = 10;
        Towns = new array<ref EoH_CaptureTownConfig>();
    }
};
