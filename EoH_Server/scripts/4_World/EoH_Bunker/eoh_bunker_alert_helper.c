class EoH_BunkerGlobalAlert
{
    static void Send(PlayerBase player)
    {
        Print("[EoH_Bunker] EoH_BunkerGlobalAlert.Send called");

        string openerName = "Unknown Survivor";
        if (player && player.GetIdentity())
            openerName = player.GetIdentity().GetName();

        string title = "BUNKER BREACH DETECTED";
        string message = openerName + " has opened the underground bunker with a punch card.";

        EoH_SendExpansionNotification(title, message);
    }

    protected static void EoH_SendExpansionNotification(string title, string message)
    {
        Print("[EoH_Bunker] Sending bunker alert: " + title + " - " + message);

        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase target = PlayerBase.Cast(man);
            if (!target)
                continue;

            PlayerIdentity identity = target.GetIdentity();
            if (!identity)
                continue;

            #ifdef EXPANSIONMODCORE
            ExpansionNotification(title, message, "set:dayz_gui image:icon_skull", COLOR_EXPANSION_NOTIFICATION_ERROR, 10).Create(identity);
            #else
            Param1<string> m_MessageParam = new Param1<string>(message);
            GetGame().RPCSingleParam(target, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, identity);
            #endif
        }
    }
}
