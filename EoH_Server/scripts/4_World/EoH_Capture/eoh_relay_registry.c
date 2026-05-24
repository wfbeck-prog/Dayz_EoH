class EoH_RelayRegistryEntry
{
    string TownName;
    vector RelayPosition;

    void EoH_RelayRegistryEntry(string townName, vector pos)
    {
        TownName = townName;
        RelayPosition = pos;
    }
}

class EoH_RelayRegistry
{
    protected static ref EoH_RelayRegistry s_Instance;
    protected ref map<string, ref EoH_RelayRegistryEntry> m_RelayMap;

    static EoH_RelayRegistry Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_RelayRegistry();

        return s_Instance;
    }

    void EoH_RelayRegistry()
    {
        m_RelayMap = new map<string, ref EoH_RelayRegistryEntry>();
        Print("[EoH_RelayRegistry] Initializing cached relay registry");
        BuildRelayCache();
    }

    void BuildRelayCache()
    {
        m_RelayMap.Clear();

        EoH_CaptureConfig config = EoH_CaptureConfig.Get();
        if (!config)
        {
            Print("[EoH_RelayRegistry][WARN] No capture config available");
            return;
        }

        if (!config.Towns)
        {
            Print("[EoH_RelayRegistry][WARN] Capture config has no towns array");
            return;
        }

        foreach (EoH_CaptureTownConfig town : config.Towns)
        {
            if (!town)
                continue;

            string townName = town.Name;
            vector relayPos = town.RadioRelayPosition;

            if (townName == "")
                continue;

            m_RelayMap.Set(townName, new EoH_RelayRegistryEntry(townName, relayPos));

            Print("[EoH_RelayRegistry] Cached relay town=" + townName + " pos=" + relayPos.ToString());
        }

        Print("[EoH_RelayRegistry] Relay cache built towns=" + m_RelayMap.Count().ToString());
    }

    bool GetRelayPosition(string townName, out vector pos)
    {
        if (!m_RelayMap.Contains(townName))
            return false;

        EoH_RelayRegistryEntry entry = m_RelayMap.Get(townName);
        if (!entry)
            return false;

        pos = entry.RelayPosition;
        return true;
    }

    bool HasRelay(string townName)
    {
        return m_RelayMap.Contains(townName);
    }
};
