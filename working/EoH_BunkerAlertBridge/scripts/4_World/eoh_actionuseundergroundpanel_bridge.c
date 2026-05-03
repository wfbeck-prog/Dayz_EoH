modded class ActionUseUndergroundPanel : ActionSingleUseBase
{
    override void OnStartServer(ActionData action_data)
    {
        super.OnStartServer(action_data);

        Land_Underground_Panel target = Land_Underground_Panel.Cast(action_data.m_Target.GetObject());
        if (!target)
            return;

        PlayerBase opener = PlayerBase.Cast(action_data.m_Player);
        if (!opener)
            return;

        PlayerIdentity identity = opener.GetIdentity();
        string openerName = "Unknown Survivor";

        if (identity)
            openerName = identity.GetName();

        string message = "[EoH] " + openerName + " has opened the bunker with a punchcard.";

        NotificationSystem.SendNotificationToPlayerIdentityExtended(
            null,
            10,
            "Bunker Alert",
            message
        );
    }
}