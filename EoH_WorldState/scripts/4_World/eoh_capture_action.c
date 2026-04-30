class EoH_ActionCaptureRelay : ActionInteractBase
{
    void EoH_ActionCaptureRelay()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
    }

    override string GetText()
    {
        return "Access Relay";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!target) return false;

        EoH_CaptureRelay_Base relay = EoH_CaptureRelay_Base.Cast(target.GetObject());
        if (!relay) return false;

        return true;
    }

    override void OnStartServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        EoH_CaptureRelay_Base relay = EoH_CaptureRelay_Base.Cast(action_data.m_Target.GetObject());

        if (!player || !relay) return;

        string town = relay.GetEoHTownName();
        if (town == "") return;

        EoH_CaptureManager.Get().StartCapture(town, player);
    }
};
