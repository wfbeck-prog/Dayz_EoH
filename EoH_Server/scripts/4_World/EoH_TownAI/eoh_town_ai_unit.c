modded class eAIBase
{
    protected string m_EoH_TownAITownName;

    void EoH_SetTownAITownName(string townName)
    {
        m_EoH_TownAITownName = townName;
    }

    string EoH_GetTownAITownName()
    {
        return m_EoH_TownAITownName;
    }

    bool EoH_IsTownAIUnit()
    {
        return m_EoH_TownAITownName != "";
    }
};
