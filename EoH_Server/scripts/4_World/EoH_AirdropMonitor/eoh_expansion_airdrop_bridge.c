modded class ExpansionMissionEventAirdrop
{
    protected bool m_EoH_AirdropLandedPosted;

    override void Event_OnStart()
    {
        super.Event_OnStart();

        string locationName = EoH_GetDropLocationName();
        vector locationPos = EoH_GetDropLocationPosition();

        EoH_AirdropMissionMonitor.Get().OnExpansionAirdropMissionStarted(MissionName, locationName, locationPos);
    }

    override void Event_OnUpdate(float delta)
    {
        super.Event_OnUpdate(delta);

        if (m_EoH_AirdropLandedPosted)
            return;

        if (m_Container && m_Container.Expansion_HasLanded())
        {
            m_EoH_AirdropLandedPosted = true;

            string locationName = EoH_GetDropLocationName();
            vector locationPos = EoH_GetDropLocationPosition();

            EoH_AirdropMissionMonitor.Get().OnExpansionAirdropLanded(MissionName, locationName, locationPos);
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
