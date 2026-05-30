class EoH_RelayRepairActionAnchor : ItemBase
{
    override void SetActions()
    {
        super.SetActions();
        Print("[EoH_RelayAction][REGISTER] EoH_RelayRepairActionAnchor SetActions adding ActionRepairAltarRelayTerminal");
        AddAction(ActionRepairAltarRelayTerminal);
    }
}
