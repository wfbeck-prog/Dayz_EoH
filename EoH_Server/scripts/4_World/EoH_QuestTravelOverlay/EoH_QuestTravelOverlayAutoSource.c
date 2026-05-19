class EoH_QuestTravelOverlayAutoSource
{
    /*
        EoH Quest Travel Overlay Auto Source

        Current behavior:
        - Loads overlay entries from:
            $profile:EoH/QuestTravelOverlays.json

        Result:
        - No code changes required for new travel/search quests.
        - Server owners only add quest/objective/position/radius entries to JSON.
        - Circle overlay system works for every configured travel objective.

        Future expansion:
        - Runtime Expansion objective discovery.
        - Automatic quest JSON parsing.
    */

    static array<ref EoH_QuestTravelOverlayData> BuildForPlayer(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return new array<ref EoH_QuestTravelOverlayData>();

        return EoH_QuestTravelOverlayFileLoader.Get();
    }

    static bool IsLikelyTravelObjectiveType(string typeName)
    {
        string lower = typeName;
        lower.ToLower();

        return lower.Contains("travel") || lower.Contains("position") || lower.Contains("location") || lower.Contains("area") || lower.Contains("search");
    }
};
