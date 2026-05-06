static const string EOH_RELAY_PROFILE_DIR = "$profile:EoH_Server";
static const string EOH_RELAY_CONFIG_PATH = "$profile:EoH_Server/Relays.json";

class EoH_RelayLocation
{
    string TownName;
    ref array<float> Position;
    float Orientation;
    bool Enabled;

    void EoH_RelayLocation()
    {
        TownName = "";
        Position = new array<float>();
        Orientation = 0.0;
        Enabled = true;
    }

    vector GetPosition()
    {
        if (!Position || Position.Count() < 3)
            return "0 0 0".ToVector();

        return Vector(Position.Get(0), Position.Get(1), Position.Get(2));
    }

    void SetPosition(vector pos)
    {
        Position.Clear();
        Position.Insert(pos[0]);
        Position.Insert(pos[1]);
        Position.Insert(pos[2]);
    }
}

class EoH_RelayConfig
{
    int Version;
    bool Enabled;
    bool SpawnRelaysOnServerStart;
    bool DeleteExistingEoHRelaysBeforeSpawn;
    ref array<ref EoH_RelayLocation> Relays;

    void EoH_RelayConfig()
    {
        Version = 1;
        Enabled = true;
        SpawnRelaysOnServerStart = false;
        DeleteExistingEoHRelaysBeforeSpawn = false;
        Relays = new array<ref EoH_RelayLocation>();
    }

    void Defaults()
    {
        SpawnRelaysOnServerStart = false;
        DeleteExistingEoHRelaysBeforeSpawn = false;
        Relays.Clear();
        AddRelay("Pustoshka", "3060 0 7870".ToVector(), 0);
        AddRelay("Mogilevka", "7600 0 5100".ToVector(), 0);
        AddRelay("Guglovo", "8500 0 6600".ToVector(), 0);
        AddRelay("Tulga", "12750 0 4400".ToVector(), 0);
        AddRelay("Nadezhdino", "5850 0 4800".ToVector(), 0);
        AddRelay("Kamenka", "1850 0 2200".ToVector(), 0);
        AddRelay("Vybor", "3850 0 8900".ToVector(), 0);
        AddRelay("Stary Sobor", "6100 0 7800".ToVector(), 0);
        AddRelay("Novy Sobor", "7000 0 7600".ToVector(), 0);
        AddRelay("Zelenogorsk", "2750 0 5300".ToVector(), 0);
        AddRelay("Staroye", "10150 0 5450".ToVector(), 0);
        AddRelay("Polana", "10700 0 8150".ToVector(), 0);
        AddRelay("Elektro", "10400 0 2250".ToVector(), 0);
        AddRelay("Chernogorsk", "6650 0 2550".ToVector(), 0);
        AddRelay("Berezino", "12250 0 9500".ToVector(), 0);
        AddRelay("NWAF", "4700 0 10300".ToVector(), 0);
        AddRelay("Tisy", "1675 0 14225".ToVector(), 0);
        AddRelay("Pavlovo Military", "2150 0 3350".ToVector(), 0);
    }

    void AddRelay(string townName, vector pos, float orientation)
    {
        EoH_RelayLocation relay = new EoH_RelayLocation();
        relay.TownName = townName;
        relay.SetPosition(pos);
        relay.Orientation = orientation;
        relay.Enabled = true;
        Relays.Insert(relay);
    }

    static EoH_RelayConfig Load()
    {
        EoH_RelayConfig cfg = new EoH_RelayConfig();

        if (!FileExist(EOH_RELAY_PROFILE_DIR))
            MakeDirectory(EOH_RELAY_PROFILE_DIR);

        if (FileExist(EOH_RELAY_CONFIG_PATH))
        {
            JsonFileLoader<EoH_RelayConfig>.JsonLoadFile(EOH_RELAY_CONFIG_PATH, cfg);
        }
        else
        {
            cfg.Defaults();
            JsonFileLoader<EoH_RelayConfig>.JsonSaveFile(EOH_RELAY_CONFIG_PATH, cfg);
        }

        return cfg;
    }
};

static ref EoH_RelayConfig g_EoH_RelayConfig;

EoH_RelayConfig GetEoHRelayConfig()
{
    if (!g_EoH_RelayConfig)
        g_EoH_RelayConfig = EoH_RelayConfig.Load();

    return g_EoH_RelayConfig;
}
