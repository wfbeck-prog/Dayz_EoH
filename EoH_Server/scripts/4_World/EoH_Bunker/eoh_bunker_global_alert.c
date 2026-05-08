class EoH_BunkerGlobalAlert
{
    protected static int s_LastAlertTime;

    static void Send(PlayerBase opener)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        int now = GetGame().GetTime();
        if (s_LastAlertTime > 0 && now - s_LastAlertTime < 30000)
            return;

        s_LastAlertTime = now;

        string openerName = "Unknown survivor";
        if (opener && opener.GetIdentity())
            openerName = opener.GetIdentity().GetName();

        string title = "BUNKER ACCESS ALERT";
        string message = openerName + " has opened the underground bunker with a punch card.";

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            PlayerIdentity identity = player.GetIdentity();

            #ifdef EXPANSIONMODCORE
            ExpansionNotification(title, message, "set:dayz_gui image:icon_skull", COLOR_EXPANSION_NOTIFICATION_ERROR, 10).Create(identity);
            #else
            player.MessageStatus(title + ": " + message);
            #endif
        }

        Print("[EoH_Bunker] " + title + ": " + message);
    }
}
