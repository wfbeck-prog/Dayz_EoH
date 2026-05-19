class EoH_QuestTravelOverlayAutoSource
{
    /*
        EoH Quest Travel Overlay Auto Source

        Goal:
        - Remove hardcoded one-off quest marker logic.
        - Generate circle-only search overlays for all valid Expansion travel/search objectives.

        Intended source priority:
        1. Runtime active Expansion objective data.
        2. Expansion quest JSON/objective JSON from profile storage.
        3. Manual fallback overlays for custom EoH search regions.

        Current safe implementation:
        - Uses fallback overlay config while preserving the automatic-source architecture.
        - This keeps the proven quest circle system stable until live Expansion objective schemas are confirmed.
    */

    static array<ref EoH_QuestTravelOverlayData> BuildForPlayer(PlayerBase player)
    {
        ref array<ref EoH_QuestTravelOverlayData> overlays = new array<ref EoH_QuestTravelOverlayData>();

        if (!player || !player.GetIdentity())
            return overlays;

        // Phase 1:
        // Use fallback config entries while auto-discovery integration is validated.
        array<ref EoH_QuestTravelOverlayData> fallback = EoH_QuestTravelOverlayConfig.GetAll();

        foreach (EoH_QuestTravelOverlayData data : fallback)
        {
            if (!data)
                continue;

            overlays.Insert(data);
        }

        return overlays;
    }

    static bool IsLikelyTravelObjectiveType(string typeName)
    {
        string lower = typeName;
        lower.ToLower();

        return lower.Contains("travel") || lower.Contains("position") || lower.Contains("location") || lower.Contains("area") || lower.Contains("search");
    }
};
