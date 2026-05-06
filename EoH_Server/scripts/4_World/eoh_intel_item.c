class EoH_Intel_Document : BookAVoyageToArcturus
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionReadIntel);
    }
}

class EoH_TownIntel : BookAVoyageToArcturus
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionReadIntel);
    }
}

class EoH_TraderIntel : BookDeadSouls
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionReadIntel);
    }
}
