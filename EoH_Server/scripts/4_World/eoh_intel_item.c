class EoH_Intel_Document : ItemBase
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionReadIntel);
    }

    override bool CanPutInCargo(EntityAI parent)
    {
        return true;
    }

    override bool CanPutIntoHands(EntityAI parent)
    {
        return true;
    }
}

class EoH_TownIntel : EoH_Intel_Document
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionReadIntel);
    }
}

class EoH_TraderIntel : EoH_Intel_Document
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionReadIntel);
    }
}

class EoH_CBDIntel : EoH_Intel_Document
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionReadIntel);
    }
}

class EoH_LootRoomIntel : EoH_CBDIntel
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionReadIntel);
    }
}

class EoH_AltarRelayIntel : EoH_Intel_Document
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionReadIntel);
    }
}

class EoH_WeekendEventIntel : EoH_Intel_Document
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionReadIntel);
    }
}
