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
        if (!target)
            return false;

        EoH_CaptureRelay_Base relay = EoH_CaptureRelay_Base.Cast(target.GetObject());
        if (!relay)
            return false;

        // enforce radius lock
        return relay.IsInsideTownRadius();
    }

    override void OnStartServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        EoH_CaptureRelay_Base relay = EoH_CaptureRelay_Base.Cast(action_data.m_Target.GetObject());

        if (!player || !relay)
            return;

        string town = relay.GetEoHTownName();
        if (town == "")
            return;

        Print("[EoH] Player " + player.GetIdentity().GetName() + " started capture at " + town);

        // simple UI feedback
        GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>("Capture Started: " + town), true, player.GetIdentity());

        EoH_CaptureManager.Get().StartCapture(town, player);
    }
};
