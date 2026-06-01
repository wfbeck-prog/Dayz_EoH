modded class ExpansionMissionEventAirdrop
{
    override void Event_OnStart()
    {
        super.Event_OnStart();

        EoH_AirdropMissionMonitor.Get().OnExpansionAirdropMissionStarted(
            MissionName,
            EoH_GetDropLocationName(),
            EoH_GetDropLocationPosition()
        );
    }

    override void Event_OnUpdate(float delta)
    {
        bool hadContainer = m_Container != null;
        bool hadLanded = false;

        if (m_Container)
            hadLanded = m_Container.Expansion_HasLanded();

        super.Event_OnUpdate(delta);

        if (!hadLanded && m_Container && m_Container.Expansion_HasLanded())
        {
            EoH_AirdropMissionMonitor.Get().OnExpansionAirdropLanded(
                MissionName,
                EoH_GetDropLocationName(),
                EoH_GetDropLocationPosition()
            );
        }
    }

    protected string EoH_GetDropLocationName()
    {
        if (DropLocation)
            return DropLocation.Name;

        return "";
    }

    protected vector EoH_GetDropLocationPosition()
    {
        if (DropLocation)
            return Vector(DropLocation.x, 0, DropLocation.z);

        return "0 0 0".ToVector();
    }
};
