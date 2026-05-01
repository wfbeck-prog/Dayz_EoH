// Trader Intel Item
// Attach this action to any intel item class you create in config.cpp

modded class Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();

        // Only attach to your intel items by classname prefix
        string type = GetType();
        if (type.Contains("EoH_Intel_Trader"))
        {
            AddAction(EoH_ActionUseTraderIntel);
        }
    }
}
