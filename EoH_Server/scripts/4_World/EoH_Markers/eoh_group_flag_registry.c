class EoH_GroupFlagRegistryEntry
{
    string GroupID;
    vector Position;
    Object FlagObject;

    void EoH_GroupFlagRegistryEntry(string groupId, vector pos, Object flagObj)
    {
        GroupID = groupId;
        Position = pos;
        FlagObject = flagObj;
    }
}

class EoH_GroupFlagRegistry
{
    protected static ref EoH_GroupFlagRegistry s_Instance;
    protected ref map<string, ref EoH_GroupFlagRegistryEntry> m_Entries;

    static EoH_GroupFlagRegistry Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_GroupFlagRegistry();

        return s_Instance;
    }

    void EoH_GroupFlagRegistry()
    {
        m_Entries = new map<string, ref EoH_GroupFlagRegistryEntry>();
        Print("[EoH_FlagRegistry] Initialized event-driven flag registry");
    }

    void RegisterFlag(string groupId, vector pos, Object flagObj)
    {
        if (groupId == "")
            return;

        EoH_GroupFlagRegistryEntry entry = new EoH_GroupFlagRegistryEntry(groupId, pos, flagObj);
        m_Entries.Set(groupId, entry);

        Print("[EoH_FlagRegistry] Registered flag group=" + groupId + " pos=" + pos.ToString());
    }

    void RemoveFlag(string groupId)
    {
        if (groupId == "")
            return;

        if (!m_Entries.Contains(groupId))
            return;

        m_Entries.Remove(groupId);
        Print("[EoH_FlagRegistry] Removed flag group=" + groupId);
    }

    bool GetFlagPosition(string groupId, out vector pos)
    {
        if (!m_Entries.Contains(groupId))
            return false;

        EoH_GroupFlagRegistryEntry entry = m_Entries.Get(groupId);
        if (!entry)
            return false;

        pos = entry.Position;
        return true;
    }

    void RegisterExistingFlag(Object flagObj, string groupId)
    {
        if (!flagObj || groupId == "")
            return;

        RegisterFlag(groupId, flagObj.GetPosition(), flagObj);
    }
};
