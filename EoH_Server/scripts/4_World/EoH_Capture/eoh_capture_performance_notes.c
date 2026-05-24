/*
    EoH PERFORMANCE MIGRATION NOTES

    Goal:
    Replace expensive world scans and repeated player loops with:

    - EoH_RelayRegistry
    - EoH_TownPlayerProximityCache
    - Event-driven registries

    Capture Manager Migration:

    OLD:
        Find live relay near town every tick
        Loop all players for every town

    NEW:
        EoH_RelayRegistry.Get().GetRelayPosition(townName, relayPos)
        EoH_TownPlayerProximityCache.Get().HasPlayerNear(townPos, radius)

    Town AI Migration:

    OLD:
        Fresh GetPlayers() calls
        Repeated proximity loops

    NEW:
        Shared cached player list refreshed every 15 seconds

    Future Optimizations:

    - Sector-based player partitioning
    - Event-driven town activation
    - Cached AI groups
    - Cached relay entities
    - Cached event objectives

    Important:
    Weekend event objectives are NOT tied to town relays.
    Weekly events use standalone EventObjectives.
*/
