class EoH_Notifications
{
    static void SendToAll(string title, string message, string icon = "set:dayz_gui image:icon_skull", int color = 0, float duration = 10.0)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (color == 0)
            color = ARGB(255, 200, 25, 25);

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player)
                continue;

            SendToPlayer(player, title, message, icon, color, duration);
        }
    }

    static void SendToPlayer(PlayerBase player, string title, string message, string icon = "set:dayz_gui image:icon_skull", int color = 0, float duration = 10.0)
    {
        if (!player || !player.GetIdentity())
            return;

        if (color == 0)
            color = ARGB(255, 200, 25, 25);

        PlayerIdentity identity = player.GetIdentity();

        #ifdef EXPANSIONMODCORE
        ExpansionNotification(title, message, icon, color, duration).Create(identity);
        #else
        player.MessageStatus(title + ": " + message);
        #endif

        Print("[EoH_Notifications] Sent: " + title + " - " + message + " to " + identity.GetName());
    }
}
