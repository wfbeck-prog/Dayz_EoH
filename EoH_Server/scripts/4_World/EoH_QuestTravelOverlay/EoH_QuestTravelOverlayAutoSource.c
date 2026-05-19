class EoH_QuestTravelOverlayAutoSource
{
    /*
        EoH Quest Travel Overlay Auto Source

        Direction:
        - Manual AddOverlay(...) entries are fallback/special-case overrides only.
        - Normal travel/search markers should eventually be discovered from active Expansion quest objective data.
        - If Expansion does not expose position/radius cleanly at runtime, this class should load the quest/objective JSON files from profile storage and build overlays from those configs.

        Intended source priority:
        1. Runtime active Expansion quest objective data, if API exposes objective position/radius.
        2. Expansion quest/objective JSON files from $profile:ExpansionMod\Quests.
        3. Manual EoH_QuestTravelOverlayConfig fallback entries for special/custom search areas.

        Current status:
        - Stub only. It intentionally returns no auto overlays until the exact Expansion objective API or JSON schema is validated on the live server.
        - This keeps the proven personal marker pipeline stable while documenting the next implementation direction.
    */

    static array<ref EoH_QuestTravelOverlayData> BuildForPlayer(PlayerBase player)
    {
        ref array<ref EoH_QuestTravelOverlayData> overlays = new array<ref EoH_QuestTravelOverlayData>();

        if (!player || !player.GetIdentity())
            return overlays;

        // TODO Phase 2:
        // Inspect active Expansion quests for this player and extract travel/search objective data.
        // Expected output per discovered objective:
        // overlays.Insert(new EoH_QuestTravelOverlayData(questId, objectiveId, label, truePosition, radius));

        // TODO Phase 3 fallback:
        // If runtime API does not expose objective coordinates, load quest/objective JSON from profile and build overlays from that config data.

        return overlays;
    }
};
