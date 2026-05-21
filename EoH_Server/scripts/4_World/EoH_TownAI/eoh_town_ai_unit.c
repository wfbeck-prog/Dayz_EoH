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

    bool EoH_IsFriendlyTownOwner(Object target)
    {
        if (!EoH_IsTownAIUnit())
            return false;

        PlayerBase player = PlayerBase.Cast(target);
        if (!player)
            return false;

        bool friendly = EoH_TownAIManager.Get().IsOwnerFriendly(player, m_EoH_TownAITownName);
        if (friendly && player.GetIdentity())
            Print("[EoH_TownAI][FRIENDLY] Owner detected near town AI town=" + m_EoH_TownAITownName + " player=" + player.GetIdentity().GetName());

        return friendly;
    }
};
