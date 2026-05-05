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
