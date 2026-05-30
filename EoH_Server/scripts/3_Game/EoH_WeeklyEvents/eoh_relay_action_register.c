modded class PlayerBase
{
    override void SetActions(out TInputActionMap InputActionMap)
    {
        super.SetActions(InputActionMap);
        Print("[EoH_RelayAction][REGISTER] PlayerBase SetActions adding ActionRepairAltarRelayTerminal");
        AddAction(ActionRepairAltarRelayTerminal, InputActionMap);
    }
}

modded class BaseRadio
{
    override void SetActions()
    {
        super.SetActions();
        Print("[EoH_RelayAction][REGISTER] BaseRadio SetActions adding ActionRepairAltarRelayTerminal type=" + GetType());
        AddAction(ActionRepairAltarRelayTerminal);
    }
}

modded class EoH_RadioRelay
{
    override void SetActions()
    {
        super.SetActions();
        Print("[EoH_RelayAction][REGISTER] EoH_RadioRelay SetActions adding ActionRepairAltarRelayTerminal type=" + GetType());
        AddAction(ActionRepairAltarRelayTerminal);
    }
}

modded class EoH_RelayRepairTerminal
{
    override void SetActions()
    {
        super.SetActions();
        Print("[EoH_RelayAction][REGISTER] EoH_RelayRepairTerminal SetActions adding ActionRepairAltarRelayTerminal type=" + GetType());
        AddAction(ActionRepairAltarRelayTerminal);
    }
}
