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
        m_IntelLocations.Clear();

        EoH_CaptureManager cap = EoH_CaptureManager.Get();
        if (!cap)
            return;

        array<string> towns = cap.GetAllTownNames();
        foreach (string town : towns)
        {
            m_IntelLocations.Set(town, cap.GetTownPos(town));
        }
    }

    void ResetPlayerIntelUsage(string playerId)
    {
        if (playerId == "" || !m_PlayerIntelUses)
            return;

        m_PlayerIntelUses.Remove(playerId);
    }

    void RevealIntel(PlayerBase player)
    {
        RevealTownIntel(player);
    }

    void RevealTownIntel(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        TrackIntelUse(player);

        vector playerPos = player.GetPosition();
        int revealed = 0;

        foreach (string name, vector pos : m_IntelLocations)
        {
            if (vector.Distance(playerPos, pos) > 5000)
                continue;

            string oldIntelId = "EoH_INTEL_TOWN_" + name;
            oldIntelId.Replace(" ", "_");
            EoH_MarkerService.RemoveFromPlayer(player, oldIntelId);

            EoH_MarkerData data = EoH_TownMarkerManager.BuildTownMarker(name, "Intel", EoH_MarkerState.NORMAL, 1, ARGB(255, 255, 220, 80));
            data.Label = name;
            data.Icon = "Info";
            data.Position = pos;
            data.Normalize();

            EoH_MarkerService.SendToPlayer(player, data);
            revealed++;
        }

        EoH_Notifications.SendToPlayer(player, "INTEL DECODED", "Nearby town activity updated: " + revealed.ToString());
    }

    void RevealTraderIntel(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        TrackIntelUse(player);

        bool revealed = EoH_RT_TraderManager.Get().RevealNearestHiddenTraderToPlayer(player);
        if (revealed)
            EoH_Notifications.SendToPlayer(player, "TRADER INTEL", "A roaming trader signal was marked.");
        else
            EoH_Notifications.SendToPlayer(player, "TRADER INTEL", "No hidden trader signal was found.");
    }

    void TrackIntelUse(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        string playerId = player.GetIdentity().GetId();
        int used = 0;
        m_PlayerIntelUses.Find(playerId, used);
        m_PlayerIntelUses.Set(playerId, used + 1);
    }
};
