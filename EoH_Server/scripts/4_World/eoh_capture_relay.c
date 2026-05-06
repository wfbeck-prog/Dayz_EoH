class EoH_CaptureRelay_Base extends Radio
{
    protected string m_EoH_TownName;

    void EoH_CaptureRelay_Base()
    {
        m_EoH_TownName = "";
    }

    override void EEInit()
    {
        super.EEInit();
        AssignNearestTown();
    }

    void AssignNearestTown()
    {
        vector myPos = GetPosition();

        EoH_CaptureManager cap = EoH_CaptureManager.Get();
        if (!cap)
            return;

        float closestDist = 999999;
        string closestTown = "";

        array<string> towns = cap.GetAllTownNames();

        foreach (string town : towns)
        {
            EoH_CaptureTownConfig cfg = cap.GetTownConfig(town);
            if (!cfg)
                continue;

            vector townPos = cfg.GetRelayVector();
            float dist = vector.Distance(myPos, townPos);

            if (dist < closestDist)
            {
                closestDist = dist;
                closestTown = town;
            }
        }

        if (closestTown != "")
        {
            m_EoH_TownName = closestTown;
            Print("[EoH] Relay assigned to town: " + closestTown);
        }
    }

    string GetEoHTownName()
    {
        if (m_EoH_TownName == "")
            AssignNearestTown();

        return m_EoH_TownName;
    }

    bool IsInsideTownRadius()
    {
        EoH_CaptureManager cap = EoH_CaptureManager.Get();
        if (!cap)
            return false;

        EoH_CaptureTownConfig cfg = cap.GetTownConfig(GetEoHTownName());
        if (!cfg)
            return false;

        float dist = vector.Distance(GetPosition(), cfg.GetRelayVector());
        return dist <= cfg.Radius;
    }

    override bool CanPutInCargo(EntityAI parent)
    {
        return false;
    }

    override bool CanPutIntoHands(EntityAI parent)
    {
        return false;
    }

    override bool CanReceiveItemIntoCargo(EntityAI item)
    {
        return false;
    }

    override bool CanReleaseCargo(EntityAI attachment)
    {
        return false;
    }

    override void SetActions()
    {
        super.SetActions();
        RemoveAction(ActionTakeItem);
        RemoveAction(ActionTakeItemToHands);
        AddAction(EoH_ActionCaptureRelay);
    }
};
