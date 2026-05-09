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
        EoH_HandleRelayActivation(action_data, "OnStartServer");
    }

    override void OnExecuteServer(ActionData action_data)
    {
        EoH_HandleRelayActivation(action_data, "OnExecuteServer");
    }

    void EoH_HandleRelayActivation(ActionData action_data, string source)
    {
        if (!action_data)
        {
            Print("[EoH_Relay][DEBUG] " + source + " failed: action_data is null");
            return;
        }

        PlayerBase player = action_data.m_Player;
        if (!player)
        {
            Print("[EoH_Relay][DEBUG] " + source + " failed: player is null");
            return;
        }

        if (!action_data.m_Target)
        {
            Print("[EoH_Relay][DEBUG] " + source + " failed: target is null for player=" + player.GetIdentity().GetName());
            return;
        }

        Object obj = action_data.m_Target.GetObject();
        if (!obj)
        {
            Print("[EoH_Relay][DEBUG] " + source + " failed: target object is null for player=" + player.GetIdentity().GetName());
            return;
        }

        Print("[EoH_Relay][DEBUG] " + source + " clicked by " + player.GetIdentity().GetName() + " targetType=" + obj.GetType() + " pos=" + obj.GetPosition().ToString());

        if (!EoH_IsRelayObject(obj.GetType()))
        {
            Print("[EoH_Relay][DEBUG] " + source + " failed: target is not relay type=" + obj.GetType());
            return;
        }

        EoH_CaptureRelay_Base relay = EoH_CaptureRelay_Base.Cast(obj);
        if (!relay)
        {
            Print("[EoH_Relay][DEBUG] " + source + " failed: relay cast failed type=" + obj.GetType());
            return;
        }

        Print("[EoH_Relay][DEBUG] " + source + " activating town=" + relay.GetEoHTownName() + " insideRadius=" + relay.IsInsideTownRadius().ToString());
        EoH_RelayGameplay.ActivateRelay(player, relay);
    }
};

bool EoH_IsRelayObject(string type)
{
    return type == "EoH_CaptureRelay_Base" || type == "EoH_RadioRelay" || type.Contains("EoH_CaptureRelay") || type.Contains("EoH_RadioRelay");
}
