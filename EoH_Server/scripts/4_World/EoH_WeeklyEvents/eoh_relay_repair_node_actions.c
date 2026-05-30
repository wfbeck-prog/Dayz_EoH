class EoH_RelayRepairNode : ItemBase
{
    override void SetActions()
    {
        super.SetActions();
        Print("[EoH_RelayAction][REGISTER] EoH_RelayRepairNode SetActions adding ActionRepairAltarRelayTerminal");
        AddAction(ActionRepairAltarRelayTerminal);
    }
}
