class EoH_ActionCaptureRelay : ActionInteractBase
{
    void EoH_ActionCaptureRelay()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = "Activate Town Relay";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTCursor;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !target)
            return false;

        Object obj = target.GetObject();
        if (!obj)
            return false;

        EoH_CaptureRelay_Base relay = EoH_CaptureRelay_Base.Cast(obj);
        if (!relay)
            return false;

        if (!relay.IsInsideTownRadius())
            return false;

        return true;
    }

    override void OnStartServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_Target)
            return;

        EoH_CaptureRelay_Base relay = EoH_CaptureRelay_Base.Cast(action_data.m_Target.GetObject());
        if (!relay)
            return;

        EoH_RelayGameplay.ActivateRelay(action_data.m_Player, relay);
    }
}
