class EoH_TownPlayerProximityCache
{
    protected static ref EoH_TownPlayerProximityCache s_Instance;

    protected ref array<Man> m_OnlinePlayers;
    protected int m_LastRefresh;

    static const int REFRESH_INTERVAL_MS = 15000;

    static EoH_TownPlayerProximityCache Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_TownPlayerProximityCache();

        return s_Instance;
    }

    void EoH_TownPlayerProximityCache()
    {
        m_OnlinePlayers = new array<Man>();
        m_LastRefresh = 0;
        Print("[EoH_PlayerCache] Initialized town player proximity cache");
    }

    void Refresh()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        int now = GetGame().GetTime();
        if (m_LastRefresh > 0 && now - m_LastRefresh < REFRESH_INTERVAL_MS)
            return;

        m_LastRefresh = now;

        m_OnlinePlayers.Clear();
        GetGame().GetPlayers(m_OnlinePlayers);

        Print("[EoH_PlayerCache] Refreshed online player cache count=" + m_OnlinePlayers.Count().ToString());
    }

    bool HasPlayerNear(vector pos, float radius)
    {
        Refresh();

        float radiusSq = radius * radius;

        foreach (Man player : m_OnlinePlayers)
        {
            PlayerBase pb = PlayerBase.Cast(player);
            if (!pb)
                continue;

            if (!pb.IsAlive())
                continue;

            vector playerPos = pb.GetPosition();
            float distSq = vector.DistanceSq(playerPos, pos);

            if (distSq <= radiusSq)
                return true;
        }

        return false;
    }
};
