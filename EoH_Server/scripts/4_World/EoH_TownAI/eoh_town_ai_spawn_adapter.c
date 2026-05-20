class EoH_TownAISpawnAdapter
{
    static Object SpawnDebugPlaceholder(string townName, vector pos)
    {
        if (!GetGame() || !GetGame().IsServer())
            return null;

        Object obj = GetGame().CreateObjectEx("WoodenStick", pos, ECE_PLACE_ON_SURFACE);
        if (obj)
        {
            obj.SetPosition(pos);
            obj.PlaceOnSurface();
            Print("[EoH_TownAI][SPAWN][DEBUG] Spawned placeholder for town=" + townName + " pos=" + pos.ToString());
        }
        else
        {
            Print("[EoH_TownAI][SPAWN][WARN] Failed to spawn debug placeholder for town=" + townName + " pos=" + pos.ToString());
        }

        return obj;
    }
};
