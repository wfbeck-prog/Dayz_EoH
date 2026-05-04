class EoH_IntelManager
{
    static ref EoH_IntelManager s_Instance;
    ref EoH_IntelConfig m_Config;

    ref map<string, int> m_LastUseTime;
    ref map<string, int> m_UsesThisLife;

    void EoH_IntelManager()
    {
        m_Config = new EoH_IntelConfig();
        m_LastUseTime = new map<string, int>();
        m_UsesThisLife = new map<string, int>();
    }

    static EoH_IntelManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_IntelManager();
        return s_Instance;
    }

    bool CanUseIntel(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return false;

        string id = player.GetIdentity().GetId();
        int now = GetGame().GetTime();

        if (m_LastUseTime.Contains(id))
        {
            int last = m_LastUseTime.Get(id);
            if ((now - last) < (m_Config.CooldownSeconds * 1000))
                return false;
        }

        if (m_UsesThisLife.Contains(id))
        {
            if (m_UsesThisLife.Get(id) >= m_Config.MaxUsesPerLife)
                return false;
        }

        return true;
    }

    void RegisterUse(PlayerBase player)
    {
        string id = player.GetIdentity().GetId();

        m_LastUseTime.Set(id, GetGame().GetTime());

        int count = 0;
        if (m_UsesThisLife.Contains(id))
            count = m_UsesThisLife.Get(id);

        m_UsesThisLife.Set(id, count + 1);
    }

    void ResetPlayerIntelUsage(string id)
    {
        m_UsesThisLife.Remove(id);
    }

    // UPDATED: trader-style marker system
    void RevealTownIntel(PlayerBase player)
    {
        if (!CanUseIntel(player))
            return;

        RegisterUse(player);

        // clear existing intel markers first
        GetGame().RPCSingleParam(
            player,
            777002,
            new Param1<int>(0),
            true,
            player.GetIdentity()
        );

        EoH_CaptureManager cap = EoH_CaptureManager.Get();
        array<string> towns = cap.GetAllTownNames();

        foreach (string town : towns)
        {
            EoH_CaptureTownConfig cfg = cap.GetTownConfig(town);
            if (!cfg)
                continue;

            vector pos = cfg.GetRelayVector();

            // send marker to client (trader-style system)
            GetGame().RPCSingleParam(
                player,
                777001,
                new Param2<vector, string>(pos, town),
                true,
                player.GetIdentity()
            );
        }

        Print("[EoH_Intel] Sent intel markers to player: " + player.GetIdentity().GetName());
    }

    void RevealHighValuePlayers(PlayerBase player)
    {
        if (!CanUseIntel(player))
            return;

        RegisterUse(player);

        // clear markers
        GetGame().RPCSingleParam(player, 777002, new Param1<int>(0), true, player.GetIdentity());

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase target = PlayerBase.Cast(man);
            if (!target || !target.GetIdentity() || !target.IsAlive())
                continue;

            int exp = EoH_DT_TerjeAdapter.GetBestSkillScore(target);
            if (exp < 2000)
                continue;

            GetGame().RPCSingleParam(
                player,
                777001,
                new Param2<vector, string>(target.GetPosition(), "High Value Target"),
                true,
                player.GetIdentity()
            );
        }

        Print("[EoH_Intel] Sent HVT intel markers");
    }

    void ClearTownIntel(PlayerBase player) {}
    void ClearPlayerIntel(PlayerBase player) {}
};
