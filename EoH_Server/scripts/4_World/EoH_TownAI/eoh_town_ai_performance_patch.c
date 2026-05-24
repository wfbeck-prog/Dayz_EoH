modded class EoH_TownAIManager
{
    override bool IsPlayerNearPosition(vector pos, float radius)
    {
        if (pos == "0 0 0".ToVector())
            return false;

        return EoH_TownPlayerProximityCache.Get().HasPlayerNear(pos, radius);
    }
}
