modded class Flag_Base
{
    protected string m_EoH_GroupID;

    void SetEoHOwner(string groupID)
    {
        m_EoH_GroupID = groupID;
        SetSynchDirty();
    }

    string GetEoHOwner()
    {
        return m_EoH_GroupID;
    }
};
