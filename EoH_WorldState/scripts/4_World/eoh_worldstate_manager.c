class EoH_WorldStateManager
{
    static ref EoH_WorldStateManager s_Instance;

    ref map<string, ref map<string, string>> m_Towns;

    void EoH_WorldStateManager()
    {
        m_Towns = new map<string, ref map<string, string>>();
        LoadState();
    }

    static EoH_WorldStateManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_WorldStateManager();
        return s_Instance;
    }

    void LoadState()
    {
        Print("[EoH_WorldState] Loading state...");
    }

    void SaveState()
    {
        Print("[EoH_WorldState] Saving state...");
    }

    void SetTownOwner(string town, string groupName)
    {
        if (!m_Towns.Contains(town))
            m_Towns.Insert(town, new map<string, string>());

        m_Towns.Get(town).Set("Owner", groupName);
        SaveState();
    }
};
