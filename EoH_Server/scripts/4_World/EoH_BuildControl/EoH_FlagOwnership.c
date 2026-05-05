modded class Flag_Base
{
    protected string m_EoH_OwnerGroupID;

    void SetEoHOwner(string groupID)
    {
        m_EoH_OwnerGroupID = groupID;
        SetSynchDirty();
    }

    string GetEoHOwner()
    {
        return m_EoH_OwnerGroupID;
    }
}
