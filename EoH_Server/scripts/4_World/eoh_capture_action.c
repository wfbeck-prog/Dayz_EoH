class EoH_ActionCaptureRelay : ActionSingleUseBase
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

    override bool HasTarget()
    {
        return false;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !item)
            return false;

        if (!EoH_IsRelayObject(item.GetType()))
            return false;

        EoH_CaptureRelay_Base relay = EoH_CaptureRelay_Base.Cast(item);
        if (!relay)
            return false;

        return relay.IsInsideTownRadius();
    }

    override void OnExecuteServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        ItemBase item = action_data.m_MainItem;

        if (!player || !item)
            return;

        if (!EoH_IsRelayObject(item.GetType()))
            return;

        EoH_CaptureRelay_Base relay = EoH_CaptureRelay_Base.Cast(item);
        if (!relay)
            return;

        EoH_RelayGameplay.ActivateRelay(player, relay);
    }
};

bool EoH_IsRelayObject(string type)
{
    return type == "EoH_CaptureRelay_Base" || type == "EoH_RadioRelay" || type.Contains("EoH_CaptureRelay") || type.Contains("EoH_RadioRelay");
}

modded class ItemBase
{
    override void SetActions()
    {
        super.SetActions();

        if (EoH_IsRelayObject(GetType()))
            AddAction(EoH_ActionCaptureRelay);
    }
};
