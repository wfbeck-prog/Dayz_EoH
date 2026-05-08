class EoH_Notifications
{
    static void SendToAll(string title, string message, string icon = "Territory", int color = 0, float duration = 10.0)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (color == 0)
            color = COLOR_EXPANSION_NOTIFICATION_MISSION;

        #ifdef EXPANSIONMOD
        // Match the proven MerkZone KOTH pattern: global Expansion notification uses Create() with no identity.
        ExpansionNotification(title, message, icon, color, duration).Create();
        Print("[EoH_Notifications] Expansion broadcast: " + title + " - " + message);
        return;
        #endif

        ref array<Man> allPlayers = new array<Man>;
        GetGame().GetPlayers(allPlayers);

        for (int i = 0; i < allPlayers.Count(); i++)
        {
            PlayerBase player = PlayerBase.Cast(allPlayers.Get(i));
            if (player && player.IsAlive())
            {
                NotificationSystem.SendNotificationToPlayerExtended(player, duration, title, message, "set:dayz_inventory image:tf_flag");
            }
        }
    }

    static void SendToPlayer(PlayerBase player, string title, string message, string icon = "Territory", int color = 0, float duration = 10.0)
    {
        if (!player)
            return;

        if (color == 0)
            color = COLOR_EXPANSION_NOTIFICATION_MISSION;

        #ifdef EXPANSIONMOD
        if (player.GetIdentity())
        {
            ExpansionNotification(title, message, icon, color, duration).Create(player.GetIdentity());
            Print("[EoH_Notifications] Expansion player notification: " + title + " - " + message);
        }
        return;
        #endif

        if (player.IsAlive())
        {
            NotificationSystem.SendNotificationToPlayerExtended(player, duration, title, message, "set:dayz_inventory image:tf_flag");
        }
    }
}
