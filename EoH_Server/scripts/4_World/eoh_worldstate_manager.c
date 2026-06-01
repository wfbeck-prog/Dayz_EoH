class EoH_WorldStateTownState
{
    string Name;
    string OwnerGroupID;
    string OwnerGroupName;
    int Tier;
    float Influence;
    int CapturedAtUnix;
    int LastChangedUnix;

    void EoH_WorldStateTownState()
    {
        Name = "";
        OwnerGroupID = "";
        OwnerGroupName = "";
        Tier = 1;
        Influence = 0.0;
        CapturedAtUnix = 0;
        LastChangedUnix = 0;
    }
};

class EoH_WorldEventState
{
    bool BunkerAwakened;
    int BunkerOpenedCount;
    int LastBunkerOpenedUnix;
    bool InfectedSurgeActive;
    bool StormCycleActive;

    void EoH_WorldEventState()
    {
        BunkerAwakened = false;
        BunkerOpenedCount = 0;
        LastBunkerOpenedUnix = 0;
        InfectedSurgeActive = false;
        StormCycleActive = false;
    }
};

class EoH_WorldStateData
{
    int WorldVersion;
    ref array<ref EoH_WorldStateTownState> Towns;
    ref EoH_WorldEventState WorldEvents;

    void EoH_WorldStateData()
    {
        WorldVersion = 1;
        Towns = new array<ref EoH_WorldStateTownState>();
        WorldEvents = new EoH_WorldEventState();
    }
};

class EoH_WorldStateManager
{
    static ref EoH_WorldStateManager s_Instance;

    protected static const string EOH_WORLDSTATE_DIR = "$profile:EoH\\WorldState";
    protected static const string EOH_WORLDSTATE_FILE = "$profile:EoH\\WorldState\\EoH_WorldState.json";

    protected ref EoH_WorldStateData m_State;

    void EoH_WorldStateManager()
    {
        LoadState();
    }

    static EoH_WorldStateManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_WorldStateManager();

        return s_Instance;
    }

    EoH_WorldStateData GetState()
    {
        return m_State;
    }

    void LoadState()
    {
        MakeDirectory("$profile:EoH");
        MakeDirectory(EOH_WORLDSTATE_DIR);

        if (FileExist(EOH_WORLDSTATE_FILE))
        {
            JsonFileLoader<EoH_WorldStateData>.JsonLoadFile(EOH_WORLDSTATE_FILE, m_State);

            if (!m_State)
            {
                Print("[EoH_WorldState] Failed to load state. Creating new default state.");
                CreateDefaultState();
                SaveState();
            }
            else
            {
                EnsureStateValid();
                Print("[EoH_WorldState] Loaded persistent world state from profile.");
            }
        }
        else
        {
            Print("[EoH_WorldState] No persistent state found. Creating new default state.");
            CreateDefaultState();
            SaveState();
        }
    }

    void SaveState()
    {
        if (!m_State)
            CreateDefaultState();

        MakeDirectory("$profile:EoH");
        MakeDirectory(EOH_WORLDSTATE_DIR);

        JsonFileLoader<EoH_WorldStateData>.JsonSaveFile(EOH_WORLDSTATE_FILE, m_State);
        Print("[EoH_WorldState] Saved persistent world state.");
    }

    protected void EnsureStateValid()
    {
        if (!m_State.Towns)
            m_State.Towns = new array<ref EoH_WorldStateTownState>();

        if (!m_State.WorldEvents)
            m_State.WorldEvents = new EoH_WorldEventState();

        if (m_State.WorldVersion <= 0)
            m_State.WorldVersion = 1;
    }

    protected void CreateDefaultState()
    {
        m_State = new EoH_WorldStateData();

        AddDefaultTown("Pustoshka", 1);
        AddDefaultTown("Mogilevka", 1);
        AddDefaultTown("Guglovo", 1);
        AddDefaultTown("Tulga", 1);
        AddDefaultTown("Nadezhdino", 1);
        AddDefaultTown("Kamenka", 1);

        AddDefaultTown("Vybor", 2);
        AddDefaultTown("Stary Sobor", 2);
        AddDefaultTown("Novy Sobor", 2);
        AddDefaultTown("Zelenogorsk", 2);
        AddDefaultTown("Staroye", 2);
        AddDefaultTown("Polana", 2);

        AddDefaultTown("Elektro", 3);
        AddDefaultTown("Chernogorsk", 3);
        AddDefaultTown("Berezino", 3);

        AddDefaultTown("NWAF", 4);
        AddDefaultTown("Tisy", 4);
        AddDefaultTown("Pavlovo Military", 4);
    }

    protected void AddDefaultTown(string name, int tier)
    {
        EoH_WorldStateTownState town = new EoH_WorldStateTownState();
        town.Name = name;
        town.Tier = tier;
        town.OwnerGroupID = "";
        town.OwnerGroupName = "Unclaimed";
        town.Influence = 0.0;
        m_State.Towns.Insert(town);
    }

    EoH_WorldStateTownState GetTownState(string townName)
    {
        if (!m_State || !m_State.Towns)
            return null;

        foreach (EoH_WorldStateTownState town : m_State.Towns)
        {
            if (town && town.Name == townName)
                return town;
        }

        return null;
    }

    void SetTownOwner(string townName, string groupID, string groupName)
    {
        if (!m_State)
            CreateDefaultState();

        EoH_WorldStateTownState town = GetTownState(townName);

        if (!town)
        {
            town = new EoH_WorldStateTownState();
            town.Name = townName;
            town.Tier = 1;
            m_State.Towns.Insert(town);
        }

        string previousOwner = town.OwnerGroupName;
        int now = GetGame().GetTime();

        town.OwnerGroupID = groupID;
        town.OwnerGroupName = groupName;
        town.Influence = 100.0;
        town.CapturedAtUnix = now;
        town.LastChangedUnix = now;

        Print("[EoH_WorldState] Town owner changed: " + townName + " -> groupID=" + groupID + " groupName=" + groupName);

        SaveState();

        EoH_WorldStateTownState verify = GetTownState(townName);
        if (verify)
            Print("[EoH_WorldState] VERIFY after save town=" + townName + " ownerID=" + verify.OwnerGroupID + " ownerName=" + verify.OwnerGroupName + " influence=" + verify.Influence.ToString());
        else
            Print("[EoH_WorldState][WARN] VERIFY failed after save. Town missing: " + townName);

        EoH_AIManager.Get().OnTownCaptured(townName, town.Tier);
        EoH_TownMarkerManager.UpdateTownMarker(townName, groupName);
        SendTerritoryWebhook(townName, previousOwner, groupName, town.Tier);
    }

    void ClearTownOwner(string townName)
    {
        EoH_WorldStateTownState town = GetTownState(townName);
        if (!town)
            return;

        string previousOwner = town.OwnerGroupName;

        Print("[EoH_WorldState] Town cleared: " + townName + " previousOwnerID=" + town.OwnerGroupID + " previousOwnerName=" + town.OwnerGroupName);

        town.OwnerGroupID = "";
        town.OwnerGroupName = "Unclaimed";
        town.Influence = 0.0;
        town.LastChangedUnix = GetGame().GetTime();

        EoH_TownMarkerManager.RemoveMarkerFromAll(EoH_TownMarkerManager.GetMarkerId(townName));

        SaveState();
        SendTerritoryWebhook(townName, previousOwner, "Unclaimed", town.Tier);
    }

    void TriggerBunkerOpened()
    {
        if (!m_State.WorldEvents)
            m_State.WorldEvents = new EoH_WorldEventState();

        m_State.WorldEvents.BunkerAwakened = true;
        m_State.WorldEvents.BunkerOpenedCount++;
        m_State.WorldEvents.LastBunkerOpenedUnix = GetGame().GetTime();
        m_State.WorldEvents.InfectedSurgeActive = true;
        m_State.WorldEvents.StormCycleActive = true;

        Print("[EoH_WorldState] Bunker opened. World event state updated.");
        SaveState();
    }

    protected void SendTerritoryWebhook(string townName, string previousOwner, string newOwner, int tier)
    {
        if (townName == "")
            return;

        if (previousOwner == newOwner)
            return;

        string title = "⚠️ TERRITORY CONTROL UPDATED";
        string body = "Relay town: " + townName + "\n";
        body += "Tier: " + tier.ToString() + "\n";
        body += "Previous control: " + previousOwner + "\n";
        body += "New control: " + newOwner + "\n";
        body += "The old relay network has logged a new ownership state.";

        EoH_DiscordWebhook.SendTerritoryConflict(title, body);
    }
};
