class EoH_CaptureSession
{
    string TownName;
    string CapturingGroupID;
    string CapturingGroupName;
    float Progress;
    int LastTick;
    bool IsContested;

    void EoH_CaptureSession()
    {
        TownName = "";
        CapturingGroupID = "";
        CapturingGroupName = "";
        Progress = 0;
        LastTick = 0;
        IsContested = false;
    }
};

class EoH_CaptureManager
{
    static ref EoH_CaptureManager s_Instance;

    ref map<string, ref EoH_CaptureSession> m_Sessions;
    ref map<string, vector> m_Towns;

    void EoH_CaptureManager()
    {
        m_Sessions = new map<string, ref EoH_CaptureSession>();
        m_Towns = new map<string, vector>();

        InitTowns();
    }

    static EoH_CaptureManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_CaptureManager();

        return s_Instance;
    }

    void InitTowns()
    {
        m_Towns.Clear();

        // Tier 1 towns
        m_Towns.Set("Pustoshka", "3060 0 7870".ToVector());
        m_Towns.Set("Mogilevka", "7600 0 5100".ToVector());
        m_Towns.Set("Guglovo", "8500 0 6600".ToVector());
        m_Towns.Set("Tulga", "12750 0 4400".ToVector());
        m_Towns.Set("Nadezhdino", "5850 0 4800".ToVector());
        m_Towns.Set("Kamenka", "1850 0 2200".ToVector());

        // Tier 2 towns
        m_Towns.Set("Vybor", "3850 0 8900".ToVector());
        m_Towns.Set("Stary Sobor", "6100 0 7800".ToVector());
        m_Towns.Set("Novy Sobor", "7000 0 7600".ToVector());
        m_Towns.Set("Zelenogorsk", "2750 0 5300".ToVector());
        m_Towns.Set("Staroye", "10150 0 5450".ToVector());
        m_Towns.Set("Polana", "10700 0 8150".ToVector());

        // Tier 3 cities
        m_Towns.Set("Elektro", "10400 0 2250".ToVector());
        m_Towns.Set("Chernogorsk", "6650 0 2550".ToVector());
        m_Towns.Set("Berezino", "12250 0 9500".ToVector());

        // Tier 4 military/high-risk zones
        m_Towns.Set("NWAF", "4700 0 10300".ToVector());
        m_Towns.Set("Tisy", "1675 0 14225".ToVector());
        m_Towns.Set("Pavlovo Military", "2150 0 3350".ToVector());
    }

    vector GetTownPos(string town)
    {
        if (m_Towns.Contains(town))
            return m_Towns.Get(town);

        EoH_CaptureTownConfig cfg = GetTownConfig(town);
        if (cfg)
            return cfg.GetRelayVector();

        return "0 0 0".ToVector();
    }

    string GetTownOwner(string town)
    {
        EoH_WorldStateTownState state = EoH_WorldStateManager.Get().GetTownState(town);
        if (!state)
            return "";

        return state.OwnerGroupName;
    }

    array<string> GetAllTownNames()
    {
        array<string> names = new array<string>();

        foreach (string town, vector pos : m_Towns)
        {
            names.Insert(town);
        }

        return names;
    }

    EoH_CaptureTownConfig GetTownConfig(string town)
    {
        if (!m_Towns.Contains(town))
            return null;

        EoH_CaptureTownConfig cfg = new EoH_CaptureTownConfig();
        cfg.Name = town;
        cfg.RelayPosition.Clear();

        vector pos = m_Towns.Get(town);
        cfg.RelayPosition.Insert(pos[0]);
        cfg.RelayPosition.Insert(pos[1]);
        cfg.RelayPosition.Insert(pos[2]);

        return cfg;
    }

    void StartCapture(string town, PlayerBase player)
    {
        if (m_Sessions.Contains(town))
            return;

        if (!player)
            return;

        string groupID = EoH_GroupHelper.GetGroupID(player);
        if (groupID == "")
        {
            player.MessageStatus("You must be in a group to capture a town.");
            return;
        }

        EoH_CaptureSession s = new EoH_CaptureSession();
        s.TownName = town;
        s.CapturingGroupID = groupID;
        s.CapturingGroupName = EoH_GroupHelper.GetGroupName(player);
        s.LastTick = GetGame().GetTime();

        m_Sessions.Set(town, s);

        EoH_TownMarkerManager.UpdateCapturingMarker(town, s.CapturingGroupName);
    }

    void Tick()
    {
        int now = GetGame().GetTime();

        foreach (string town, EoH_CaptureSession s : m_Sessions)
        {
            int delta = now - s.LastTick;
            s.LastTick = now;

            UpdatePresence(s);

            if (!s.IsContested)
            {
                s.Progress += delta;

                if (s.Progress > 600000) // 10 min
                {
                    CompleteCapture(s);
                }
            }
        }
    }

    void UpdatePresence(EoH_CaptureSession s)
    {
        vector pos = GetTownPos(s.TownName);

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        bool enemyPresent = false;

        foreach (Man m : players)
        {
            PlayerBase p = PlayerBase.Cast(m);
            if (!p || !p.GetIdentity())
                continue;

            if (vector.Distance(p.GetPosition(), pos) >= 150)
                continue;

            string otherGroupID = EoH_GroupHelper.GetGroupID(p);
            if (otherGroupID != "" && otherGroupID != s.CapturingGroupID)
            {
                enemyPresent = true;
                break;
            }
        }

        bool wasContested = s.IsContested;
        s.IsContested = enemyPresent;

        if (s.IsContested)
        {
            EoH_TownMarkerManager.UpdateContestedMarker(s.TownName, s.CapturingGroupName);
        }
        else if (wasContested)
        {
            EoH_TownMarkerManager.UpdateCapturingMarker(s.TownName, s.CapturingGroupName);
        }
    }

    void CompleteCapture(EoH_CaptureSession s)
    {
        EoH_WorldStateManager.Get().SetTownOwner(s.TownName, s.CapturingGroupID, s.CapturingGroupName);
        m_Sessions.Remove(s.TownName);
    }
};
