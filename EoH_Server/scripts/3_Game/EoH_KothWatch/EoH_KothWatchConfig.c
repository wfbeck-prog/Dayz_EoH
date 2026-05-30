class EoH_KothWatchZone
{
    string Name;
    ref array<float> Position;
    float Radius;
    string NearestTown;
    ref array<float> NearestTownRelayPosition;

    void EoH_KothWatchZone()
    {
        Name = "";
        Position = new array<float>();
        Radius = 500.0;
        NearestTown = "";
        NearestTownRelayPosition = new array<float>();
    }

    vector GetPosition()
    {
        if (!Position || Position.Count() < 3)
            return "0 0 0".ToVector();

        return Vector(Position.Get(0), Position.Get(1), Position.Get(2));
    }

    vector GetNearestTownRelayPosition()
    {
        if (!NearestTownRelayPosition || NearestTownRelayPosition.Count() < 3)
            return "0 0 0".ToVector();

        return Vector(NearestTownRelayPosition.Get(0), NearestTownRelayPosition.Get(1), NearestTownRelayPosition.Get(2));
    }
}

class EoH_KothWatchConfig
{
    int ConfigVersion = 2;
    bool Enabled = true;
    bool WarnOnly = true;
    bool LogPlayerProximity = true;
    bool LogDisconnectsNearZones = true;
    bool LogConnectsNearZones = true;
    float PlayerCheckSeconds = 30.0;
    float DisconnectRiskRadius = 750.0;

    bool EnableKothSafeMode = false;
    bool DisableTownAINearKoth = false;
    bool SuppressEoHMarkersNearKoth = false;
    float SafeModeRadius = 750.0;

    ref array<ref EoH_KothWatchZone> Zones;

    void EoH_KothWatchConfig()
    {
        Zones = new array<ref EoH_KothWatchZone>();
    }

    void Defaults()
    {
        ConfigVersion = 2;
        Enabled = true;
        WarnOnly = true;
        LogPlayerProximity = true;
        LogDisconnectsNearZones = true;
        LogConnectsNearZones = true;
        PlayerCheckSeconds = 30.0;
        DisconnectRiskRadius = 750.0;

        EnableKothSafeMode = false;
        DisableTownAINearKoth = false;
        SuppressEoHMarkersNearKoth = false;
        SafeModeRadius = 750.0;

        Zones = new array<ref EoH_KothWatchZone>();
        AddDefaultElektroZone();
    }

    void AddDefaultElektroZone()
    {
        EoH_KothWatchZone zone = new EoH_KothWatchZone();
        zone.Name = "Elektro_KOTH_Risk";
        zone.Radius = 750.0;
        zone.NearestTown = "Elektro";

        zone.Position.Insert(10458.9);
        zone.Position.Insert(6.0);
        zone.Position.Insert(2263.8);

        zone.NearestTownRelayPosition.Insert(10476.38);
        zone.NearestTownRelayPosition.Insert(6.83);
        zone.NearestTownRelayPosition.Insert(2353.72);

        Zones.Insert(zone);
    }
}
