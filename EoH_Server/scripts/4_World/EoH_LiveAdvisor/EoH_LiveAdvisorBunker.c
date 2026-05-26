class EoH_LiveAdvisorBunker
{
    static void LogBunkerOpened(PlayerBase player, string location = "Unknown Bunker")
    {
        string playerName = "Unknown Player";
        string playerId = "Unknown ID";
        string positionText = "Unknown Position";

        if (player)
        {
            playerName = player.GetIdentityName();

            if (player.GetIdentity())
            {
                playerId = player.GetIdentity().GetPlainId();
            }

            vector pos = player.GetPosition();
            positionText = string.Format("%1 %2 %3", pos[0], pos[1], pos[2]);
        }

        string message = string.Format("Bunker opened at %1 by %2 (%3). Player position: %4", location, playerName, playerId, positionText);
        EoH_LiveAdvisorLogger.Log("BUNKER_OPENED", message, "warning", "EoH_Bunker");
    }

    static void LogPunchcardUsed(PlayerBase player, string location = "Unknown Bunker Panel")
    {
        string playerName = "Unknown Player";
        string playerId = "Unknown ID";
        string positionText = "Unknown Position";

        if (player)
        {
            playerName = player.GetIdentityName();

            if (player.GetIdentity())
            {
                playerId = player.GetIdentity().GetPlainId();
            }

            vector pos = player.GetPosition();
            positionText = string.Format("%1 %2 %3", pos[0], pos[1], pos[2]);
        }

        string message = string.Format("Punchcard interaction at %1 by %2 (%3). Player position: %4", location, playerName, playerId, positionText);
        EoH_LiveAdvisorLogger.Log("PUNCHCARD_USED", message, "warning", "EoH_Bunker");
    }
}
