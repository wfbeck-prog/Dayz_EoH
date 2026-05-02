// EoH_RoamingTrader - Territory Flag Auto-Build Patch
// Purpose: When a TerritoryFlagKit is placed, it spawns a completed TerritoryFlag
// instead of requiring the player to manually build every flag part.

modded class TerritoryFlag
{
    override string GetConstructionKitType()
    {
        // Returning empty prevents the completed flag from turning back into a kit.
        return string.Empty;
    }
}
