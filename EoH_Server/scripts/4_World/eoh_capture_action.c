class EoH_ActionCaptureRelay : ActionInteractBase
{
    void EoH_ActionCaptureRelay()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
    }

    override string GetText()
    {
        return "Activate Relay";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !target)
            return false;

        Object obj = target.GetObject();
        if (!obj || !EoH_IsRelayObject(obj.GetType()))
            return false;

        EoH_CaptureRelay_Base relay = EoH_CaptureRelay_Base.Cast(obj);
        if (!relay)
            return false;

        if (!relay.IsInsideTownRadius())
            Print("[EoH_Relay][DEBUG] Activate Relay visible, but relay reports outside town radius. Type=" + obj.GetType() + " pos=" + obj.GetPosition().ToString());

        return true;
    }

    override void OnStartServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        if (!player || !action_data.m_Target)
            return;

        Object obj = action_data.m_Target.GetObject();
        if (!obj || !EoH_IsRelayObject(obj.GetType()))
            return;

        EoH_CaptureRelay_Base relay = EoH_CaptureRelay_Base.Cast(obj);
        if (!relay)
            return;

        EoH_RelayGameplay.ActivateRelay(player, relay);
    }
};

bool EoH_IsRelayObject(string type)
{
    return type == "EoH_CaptureRelay_Base" || type == "EoH_RadioRelay" || type.Contains("EoH_CaptureRelay") || type.Contains("EoH_RadioRelay");
}
