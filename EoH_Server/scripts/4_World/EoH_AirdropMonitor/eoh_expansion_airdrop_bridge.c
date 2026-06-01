modded class ExpansionMissionEventAirdrop
{
    protected bool m_EoH_AirdropStartPosted;
    protected bool m_EoH_AirdropLandedPosted;

    override void Event_OnStart()
    {
        super.Event_OnStart();

        if (!IsMissionHost())
            return;

        if (m_EoH_AirdropStartPosted)
            return;

        m_EoH_AirdropStartPosted = true;
        EoH_AirdropMissionMonitor.Get().OnExpansionAirdropMissionStarted(MissionName, EoH_GetDropLocationName(), EoH_GetDropLocationPosition());
    }

    override void Event_OnUpdate(float delta)
    {
        super.Event_OnUpdate(delta);

        if (!IsMissionHost())
            return;

        if (m_EoH_AirdropLandedPosted)
            return;

        if (m_Container && m_Container.Expansion_HasLanded())
        {
            m_EoH_AirdropLandedPosted = true;
            EoH_AirdropMissionMonitor.Get().OnExpansionAirdropLanded(MissionName, EoH_GetDropLocationName(), EoH_GetDropLocationPosition());
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
        {
            vector pos;
            pos[0] = DropLocation.x;
            pos[1] = 0;
            pos[2] = DropLocation.z;
            return pos;
        }

        return "0 0 0".ToVector();
    }
};
