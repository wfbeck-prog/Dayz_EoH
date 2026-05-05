class EoH_SpawnProtection
{
    static ref map<string, int> m_LastSpawnTime;

    static bool CanSpawn(PlayerBase player, vector flagPos, out string reason)
    {
        string groupID = EoH_GroupHelper.GetGroupID(player);

        if (groupID == "")
        {
            reason = "You must be in a group.";
            return false;
        }

        if (!m_LastSpawnTime)
            m_LastSpawnTime = new map<string, int>();

        int now = GetGame().GetTime();
        int cooldown = 300000; // 5 minutes

        if (m_LastSpawnTime.Contains(groupID))
        {
            int last = m_LastSpawnTime.Get(groupID);
            if (now - last < cooldown)
            {
                int remaining = (cooldown - (now - last)) / 1000;
                reason = "Spawn cooldown active: " + remaining.ToString() + "s";
                return false;
            }
        }

        if (IsEnemyNearby(player, flagPos))
        {
            reason = "Enemies detected near your territory.";
            return false;
        }

        return true;
    }

    static void RegisterSpawn(PlayerBase player)
    {
        string groupID = EoH_GroupHelper.GetGroupID(player);
        if (groupID == "")
            return;

        if (!m_LastSpawnTime)
            m_LastSpawnTime = new map<string, int>();

        m_LastSpawnTime.Set(groupID, GetGame().GetTime());
    }

    static bool IsEnemyNearby(PlayerBase player, vector pos)
    {
        float radius = 60.0;

        array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition(pos, radius, objects, null);

        string myGroup = EoH_GroupHelper.GetGroupID(player);

        foreach (Object obj : objects)
        {
            PlayerBase other = PlayerBase.Cast(obj);
            if (!other)
                continue;

            string otherGroup = EoH_GroupHelper.GetGroupID(other);

            if (otherGroup != "" && otherGroup != myGroup)
                return true;
        }

        return false;
    }
};
