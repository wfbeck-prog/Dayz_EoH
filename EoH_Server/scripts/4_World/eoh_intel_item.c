class EoH_Intel_Document : Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionUseIntel);
    }
};
