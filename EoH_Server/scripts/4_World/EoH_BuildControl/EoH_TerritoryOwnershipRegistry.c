static const string EOH_TERRITORY_PROFILE_DIR = "$profile:EoH_Server";
static const string EOH_TERRITORY_OWNERSHIP_PATH = "$profile:EoH_Server/Territories.json";

class EoH_TerritoryOwnershipEntry
{
    string Key;
    string OwnerGroupID;
    ref array<float> Position;

    void EoH_TerritoryOwnershipEntry()
    {
        Key = "";
        OwnerGroupID = "";
        Position = new array<float>();
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

class EoH_TerritoryOwnershipFile
{
    int Version;
    ref array<ref EoH_TerritoryOwnershipEntry> Territories;

    void EoH_TerritoryOwnershipFile()
    {
        Version = 1;
        Territories = new array<ref EoH_TerritoryOwnershipEntry>();
    }
}

class EoH_TerritoryOwnershipRegistry
{
    protected static ref map<string, string> s_FlagOwners;
    protected static ref map<string, vector> s_FlagPositions;
    protected static bool s_Loaded;

    static void Ensure()
    {
        if (!s_FlagOwners)
            s_FlagOwners = new map<string, string>();

        if (!s_FlagPositions)
            s_FlagPositions = new map<string, vector>();

        if (!s_Loaded)
            Load();
    }

    static void SetOwnerAtPosition(vector pos, string ownerGroupID)
    {
        Ensure();

        string key = MakePositionKey(pos);
        s_FlagOwners.Set(key, ownerGroupID);
        s_FlagPositions.Set(key, pos);

        Save();
    }

    static string GetOwnerAtPosition(vector pos)
    {
        Ensure();
        string key = MakePositionKey(pos);
        if (s_FlagOwners.Contains(key))
            return s_FlagOwners.Get(key);

        return "";
    }

    static string GetOwnerForObject(Object obj)
    {
        if (!obj)
            return "";

        return GetOwnerAtPosition(obj.GetPosition());
    }

    static ref array<ref EoH_TerritoryOwnershipEntry> GetAllTerritories()
    {
        Ensure();

        array<ref EoH_TerritoryOwnershipEntry> result = new array<ref EoH_TerritoryOwnershipEntry>();

        foreach (string key, string owner : s_FlagOwners)
        {
            EoH_TerritoryOwnershipEntry entry = new EoH_TerritoryOwnershipEntry();
            entry.Key = key;
            entry.OwnerGroupID = owner;

            if (s_FlagPositions.Contains(key))
                entry.SetPosition(s_FlagPositions.Get(key));

            result.Insert(entry);
        }

        return result;
    }

    static void Load()
    {
        if (!s_FlagOwners)
            s_FlagOwners = new map<string, string>();

        if (!s_FlagPositions)
            s_FlagPositions = new map<string, vector>();

        s_FlagOwners.Clear();
        s_FlagPositions.Clear();

        if (!FileExist(EOH_TERRITORY_PROFILE_DIR))
            MakeDirectory(EOH_TERRITORY_PROFILE_DIR);

        if (!FileExist(EOH_TERRITORY_OWNERSHIP_PATH))
        {
            Save();
            s_Loaded = true;
            return;
        }

        EoH_TerritoryOwnershipFile file = new EoH_TerritoryOwnershipFile();
        JsonFileLoader<EoH_TerritoryOwnershipFile>.JsonLoadFile(EOH_TERRITORY_OWNERSHIP_PATH, file);

        foreach (EoH_TerritoryOwnershipEntry entry : file.Territories)
        {
            if (!entry || entry.Key == "" || entry.OwnerGroupID == "")
                continue;

            s_FlagOwners.Set(entry.Key, entry.OwnerGroupID);
            s_FlagPositions.Set(entry.Key, entry.GetPosition());
        }

        s_Loaded = true;
    }

    static void Save()
    {
        if (!s_FlagOwners)
            s_FlagOwners = new map<string, string>();

        if (!s_FlagPositions)
            s_FlagPositions = new map<string, vector>();

        if (!FileExist(EOH_TERRITORY_PROFILE_DIR))
            MakeDirectory(EOH_TERRITORY_PROFILE_DIR);

        EoH_TerritoryOwnershipFile file = new EoH_TerritoryOwnershipFile();

        foreach (string key, string owner : s_FlagOwners)
        {
            EoH_TerritoryOwnershipEntry entry = new EoH_TerritoryOwnershipEntry();
            entry.Key = key;
            entry.OwnerGroupID = owner;

            if (s_FlagPositions.Contains(key))
                entry.SetPosition(s_FlagPositions.Get(key));

            file.Territories.Insert(entry);
        }

        JsonFileLoader<EoH_TerritoryOwnershipFile>.JsonSaveFile(EOH_TERRITORY_OWNERSHIP_PATH, file);
    }

    static string MakePositionKey(vector pos)
    {
        int x = Math.Round(pos[0]);
        int z = Math.Round(pos[2]);
        return x.ToString() + "_" + z.ToString();
    }
}
