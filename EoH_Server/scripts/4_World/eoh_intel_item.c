class EoH_Intel_Document : ItemBase
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionReadIntel);
    }
}

class EoH_TownIntel : EoH_Intel_Document
{
}

class EoH_TraderIntel : EoH_Intel_Document
{
}

class EoH_CaptureRelay_Base : ItemBase
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionCaptureRelay);
    }
}

class EoH_RadioRelay : EoH_CaptureRelay_Base
{
}
