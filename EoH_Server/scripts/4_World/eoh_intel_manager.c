class EoH_IntelManager
{
    static ref EoH_IntelManager s_Instance;

    ref map<string, vector> m_IntelLocations;
    ref map<string, int> m_PlayerIntelUses;

    void EoH_IntelManager()
    {
        m_IntelLocations = new map<string, vector>();
        m_PlayerIntelUses = new map<string, int>();
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
        m_IntelLocations.Set("Pustoshka", "3060 0 7870".ToVector());
        m_IntelLocations.Set("Mogilevka", "7600 0 5100".ToVector());
        m_IntelLocations.Set("Guglovo", "8500 0 6600".ToVector());
        m_IntelLocations.Set("Novy Sobor", "7000 0 7600".ToVector());
    }

    void ResetPlayerIntelUsage(string playerId)
    {
        if (playerId == "" || !m_PlayerIntelUses)
            return;

        m_PlayerIntelUses.Remove(playerId);
    }

    void RevealTownIntel(PlayerBase player)
    {
        RevealIntel(player);
    }

    void RevealIntel(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        string playerId = player.GetIdentity().GetId();
        int used = 0;
        m_PlayerIntelUses.Find(playerId, used);
        m_PlayerIntelUses.Set(playerId, used + 1);

        vector playerPos = player.GetPosition();

        foreach (string name, vector pos : m_IntelLocations)
        {
            if (vector.Distance(playerPos, pos) > 3000)
                continue;

            EoH_MarkerData data = new EoH_MarkerData("EoH_INTEL_" + name, "Intel: " + name, pos);
            data.Category = EoH_MarkerCategory.INTEL;
            data.State = EoH_MarkerState.NORMAL;
            data.Icon = "Info";
            data.Is3D = 0;
            data.Pulse = 1;
            data.Color = ARGB(255, 255, 220, 80);
            data.BaseColor = data.Color;
            data.Normalize();

            EoH_MarkerService.SendToPlayer(player, data);
        }
    }
};
