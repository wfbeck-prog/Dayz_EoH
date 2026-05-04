class EoH_CaptureSession
{
    string TownName;
    string AttackingGroupID;
    string AttackingGroupName;
    float Progress;
    int LastTickTime;
    bool IsContested;
    int AttackersNearby;
    int EnemiesNearby;
};

class EoH_CaptureManager
{
    static ref EoH_CaptureManager s_Instance;

    ref map<string, ref EoH_CaptureSession> m_ActiveCaptures;
    ref EoH_CaptureConfig m_Config;

    void EoH_CaptureManager()
    {
        m_ActiveCaptures = new map<string, ref EoH_CaptureSession>();
    }

    static EoH_CaptureManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_CaptureManager();

        return s_Instance;
    }

    void Tick()
    {
        array<string> keys = m_ActiveCaptures.GetKeyArray();

        foreach (string town : keys)
        {
            EoH_CaptureSession session = m_ActiveCaptures.Get(town);
            if (!session) continue;

            EoH_CaptureTownConfig cfg = GetTownConfig(town);
            if (!cfg) continue;

            UpdatePresence(session, cfg);
        }
    }

    void UpdatePresence(EoH_CaptureSession session, EoH_CaptureTownConfig townCfg)
    {
        bool wasContested = session.IsContested;

        if (session.EnemiesNearby > 0)
            session.IsContested = true;
        else
            session.IsContested = false;

        vector pos = townCfg.GetRelayVector();

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        if (session.IsContested)
        {
            foreach (Man man : players)
            {
                PlayerBase player = PlayerBase.Cast(man);
                if (!player || !player.GetIdentity()) continue;

                if (vector.Distance(player.GetPosition(), pos) <= townCfg.Radius * 1.5)
                {
                    GetGame().RPCSingleParam(
                        player,
                        777003,
                        new Param2<vector, string>(pos, session.TownName),
                        true,
                        player.GetIdentity()
                    );
                }
            }
        }
        else if (wasContested)
        {
            foreach (Man man : players)
            {
                PlayerBase player = PlayerBase.Cast(man);
                if (!player || !player.GetIdentity()) continue;

                if (vector.Distance(player.GetPosition(), pos) <= townCfg.Radius * 1.5)
                {
                    GetGame().RPCSingleParam(
                        player,
                        777001,
                        new Param3<vector, string, int>(pos, session.TownName, ARGB(120,150,150,150)),
                        true,
                        player.GetIdentity()
                    );
                }
            }
        }
    }

    EoH_CaptureTownConfig GetTownConfig(string town)
    {
        return null; // placeholder safe compile
    }
};
