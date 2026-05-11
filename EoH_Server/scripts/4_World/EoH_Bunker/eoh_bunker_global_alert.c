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

        string title = "BUNKER ACCESS ALERT";
        string message = "A survivor has opened the underground bunker with a punch card.";

        EoH_Notifications.SendToAll(title, message);
        EoH_DiscordWebhook.SendBunkerOpened(opener);

        Print("[EoH_Bunker] " + title + ": " + message);
    }
}
