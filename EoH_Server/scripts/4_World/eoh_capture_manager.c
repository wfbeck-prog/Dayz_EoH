class EoH_CaptureSession
{
    string TownName;
    float Progress;
    int LastTick;
    bool IsContested;

    void EoH_CaptureSession()
    {
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
        m_Towns.Set("Pustoshka", "3060 0 7870".ToVector());
        m_Towns.Set("Mogilevka", "7600 0 5100".ToVector());
        m_Towns.Set("Guglovo", "8500 0 6600".ToVector());
        m_Towns.Set("Novy Sobor", "7000 0 7600".ToVector());
    }

    vector GetTownPos(string town)
    {
        if (m_Towns.Contains(town))
            return m_Towns.Get(town);

        return "0 0 0".ToVector();
    }

    void StartCapture(string town, PlayerBase player)
    {
        if (m_Sessions.Contains(town))
            return;

        EoH_CaptureSession s = new EoH_CaptureSession();
        s.TownName = town;
        s.LastTick = GetGame().GetTime();

        m_Sessions.Set(town, s);

        SendMarker(town, ARGB(255,255,220,80));
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

                if (s.Progress > 600000)
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

        int count = 0;

        foreach (Man m : players)
        {
            PlayerBase p = PlayerBase.Cast(m);
            if (!p || !p.GetIdentity()) continue;

            if (vector.Distance(p.GetPosition(), pos) < 150)
                count++;
        }

        bool was = s.IsContested;
        s.IsContested = count > 1;

        if (s.IsContested)
        {
            SendContested(s.TownName);
        }
        else if (was)
        {
            SendMarker(s.TownName, ARGB(255,255,220,80));
        }
    }

    void CompleteCapture(EoH_CaptureSession s)
    {
        SendMarker(s.TownName, ARGB(255,80,255,120));
        m_Sessions.Remove(s.TownName);
    }

    void SendMarker(string town, int color)
    {
        vector pos = GetTownPos(town);

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man m : players)
        {
            PlayerBase p = PlayerBase.Cast(m);
            if (!p || !p.GetIdentity()) continue;

            GetGame().RPCSingleParam(
                p,
                777001,
                new Param3<vector, string, int>(pos, town, color),
                true,
                p.GetIdentity()
            );
        }
    }

    void SendContested(string town)
    {
        vector pos = GetTownPos(town);

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man m : players)
        {
            PlayerBase p = PlayerBase.Cast(m);
            if (!p || !p.GetIdentity()) continue;

            GetGame().RPCSingleParam(
                p,
                777003,
                new Param2<vector, string>(pos, town),
                true,
                p.GetIdentity()
            );
        }
    }
};
