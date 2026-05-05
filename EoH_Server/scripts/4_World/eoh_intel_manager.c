class EoH_IntelManager
{
    static ref EoH_IntelManager s_Instance;

    ref map<string, vector> m_IntelLocations;

    void EoH_IntelManager()
    {
        m_IntelLocations = new map<string, vector>();
        InitIntel();
    }

    static EoH_IntelManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_IntelManager();

        return s_Instance;
    }

    void InitIntel()
    {
        // 🔧 Define intel-revealable locations here
        m_IntelLocations.Set("Pustoshka", "3060 0 7870".ToVector());
        m_IntelLocations.Set("Mogilevka", "7600 0 5100".ToVector());
        m_IntelLocations.Set("Guglovo", "8500 0 6600".ToVector());
        m_IntelLocations.Set("Novy Sobor", "7000 0 7600".ToVector());
    }

    // 🔥 MAIN ENTRY POINT (called from item / action)
    void RevealIntel(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        vector playerPos = player.GetPosition();

        foreach (string name, vector pos : m_IntelLocations)
        {
            // Optional: limit by distance
            if (vector.Distance(playerPos, pos) > 3000)
                continue;

            // 🔥 USE UNIFIED MARKER SYSTEM
            EoH_MarkerManager.SendToNearby(name, pos, 777002, 3000);
        }
    }
};
