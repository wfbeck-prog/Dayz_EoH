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

        string message = "BUNKER ACCESS ALERT: " + openerName + " has opened the underground bunker with a punch card.";

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            player.MessageStatus(message);
        }

        Print("[EoH_Bunker] " + message);
    }
}
