class EoH_CaptureSession
{
    string TownName;
    string CapturingGroupID;
    string CapturingGroupName;
    float Progress;
    int LastTick;
    int LastNotifyPercent;
    bool IsContested;
    bool CapturingGroupPresent;
    bool WasPausedNoPresence;

    void EoH_CaptureSession()
    {
        TownName = "";
        CapturingGroupID = "";
        CapturingGroupName = "";
        Progress = 0;
        LastTick = 0;
        LastNotifyPercent = 0;
        IsContested = false;
        CapturingGroupPresent = false;
        WasPausedNoPresence = false;
    }
};

class EoH_CaptureManager
{
    static ref EoH_CaptureManager s_Instance;
    static const float CAPTURE_DURATION_MS = 600000.0;
    static const float CAPTURE_RADIUS = 150.0;
    static const bool EOH_CAPTURE_DEBUG = true;

    ref map<string, ref EoH_CaptureSession> m_Sessions;
    ref map<string, vector> m_Towns;
    protected int m_LastDebugTick;

    void EoH_CaptureManager()
    {
        m_Sessions = new map<string, ref EoH_CaptureSession>();
        m_Towns = new map<string, vector>();
        m_LastDebugTick = 0;

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
        m_Towns.Set("Pustoshka", "3060 0 7870".ToVector());
        m_Towns.Set("Mogilevka", "7600 0 5100".ToVector());
        m_Towns.Set("Guglovo", "8500 0 6600".ToVector());
        m_Towns.Set("Tulga", "12750 0 4400".ToVector());
        m_Towns.Set("Nadezhdino", "5850 0 4800".ToVector());
        m_Towns.Set("Kamenka", "1850 0 2200".ToVector());
        m_Towns.Set("Vybor", "3850 0 8900".ToVector());
        m_Towns.Set("Stary Sobor", "6100 0 7800".ToVector());
        m_Towns.Set("Novy Sobor", "7000 0 7600".ToVector());
        m_Towns.Set("Zelenogorsk", "2750 0 5300".ToVector());
        m_Towns.Set("Staroye", "10150 0 5450".ToVector());
        m_Towns.Set("Polana", "10700 0 8150".ToVector());
        m_Towns.Set("Elektro", "10400 0 2250".ToVector());
        m_Towns.Set("Chernogorsk", "6650 0 2550".ToVector());
        m_Towns.Set("Berezino", "12250 0 9500".ToVector());
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
            names.Insert(town);
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
        {
            if (player)
                EoH_Notifications.SendToPlayer(player, "TOWN CAPTURE", town + " is already being captured.");
            return;
        }

        if (!player)
            return;

        vector townPos = GetTownPos(town);
        if (vector.Distance(player.GetPosition(), townPos) > CAPTURE_RADIUS)
        {
            EoH_Notifications.SendToPlayer(player, "TOWN CAPTURE FAILED", "You must be inside the capture zone to start capturing " + town + ".");
            Print("[EoH_Capture] Blocked start outside radius town=" + town + " player=" + player.GetIdentity().GetName() + " dist=" + vector.Distance(player.GetPosition(), townPos).ToString() + " playerPos=" + player.GetPosition().ToString() + " townPos=" + townPos.ToString());
            return;
        }

        string groupID = EoH_GroupHelper.GetGroupID(player);
        if (groupID == "")
        {
            EoH_Notifications.SendToPlayer(player, "TOWN CAPTURE FAILED", "You must be in a group to capture a town.");
            Print("[EoH_Capture] Blocked start no group town=" + town + " player=" + player.GetIdentity().GetName());
            return;
        }

        EoH_CaptureSession s = new EoH_CaptureSession();
        s.TownName = town;
        s.CapturingGroupID = groupID;
        s.CapturingGroupName = EoH_GroupHelper.GetGroupName(player);
        s.LastTick = GetGame().GetTime();
        s.LastNotifyPercent = 0;
        s.CapturingGroupPresent = true;

        m_Sessions.Set(town, s);

        EoH_TownMarkerManager.UpdateCapturingMarker(town, s.CapturingGroupName);
        UpdateCaptureProgressMarker(s, 0);
        BroadcastCaptureMessage("TOWN CAPTURE STARTED", s.CapturingGroupName + " started capturing " + town + ". Hold for 10 minutes.");
        Print("[EoH_Capture] Started capture town=" + town + " group=" + s.CapturingGroupName + " playerPos=" + player.GetPosition().ToString() + " townPos=" + townPos.ToString());
    }

    void Tick()
    {
        int now = GetGame().GetTime();

        if (EOH_CAPTURE_DEBUG && now - m_LastDebugTick >= 30000)
        {
            m_LastDebugTick = now;
            Print("[EoH_Capture][DEBUG] Tick sessions=" + m_Sessions.Count().ToString() + " towns=" + m_Towns.Count().ToString());
        }

        foreach (string town, EoH_CaptureSession s : m_Sessions)
        {
            int delta = now - s.LastTick;
            s.LastTick = now;

            UpdatePresence(s);

            if (EOH_CAPTURE_DEBUG)
                Print("[EoH_Capture][DEBUG] Session town=" + s.TownName + " group=" + s.CapturingGroupName + " present=" + s.CapturingGroupPresent.ToString() + " contested=" + s.IsContested.ToString() + " progressMs=" + s.Progress.ToString() + " delta=" + delta.ToString());

            if (!s.CapturingGroupPresent)
                continue;

            if (!s.IsContested)
            {
                s.Progress += delta;

                int percent = Math.Clamp(Math.Floor((s.Progress / CAPTURE_DURATION_MS) * 100.0), 0, 100);
                int bucket = Math.Floor(percent / 10) * 10;

                if (bucket >= s.LastNotifyPercent + 10 || percent >= 100)
                {
                    s.LastNotifyPercent = bucket;
                    UpdateCaptureProgressMarker(s, percent);
                    BroadcastCaptureMessage("TOWN CAPTURE", s.TownName + " capture progress: " + percent.ToString() + "%");
                    Print("[EoH_Capture] Progress town=" + s.TownName + " group=" + s.CapturingGroupName + " percent=" + percent.ToString());
                }

                if (s.Progress > CAPTURE_DURATION_MS)
                    CompleteCapture(s);
            }
        }
    }

    void UpdatePresence(EoH_CaptureSession s)
    {
        vector pos = GetTownPos(s.TownName);
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        bool enemyPresent = false;
        bool captureGroupPresent = false;
        int playersInRadius = 0;
        int captureGroupInRadius = 0;
        int enemiesInRadius = 0;

        foreach (Man m : players)
        {
            PlayerBase p = PlayerBase.Cast(m);
            if (!p || !p.GetIdentity() || !p.IsAlive())
                continue;

            float dist = vector.Distance(p.GetPosition(), pos);
            if (dist >= CAPTURE_RADIUS)
                continue;

            playersInRadius++;
            string otherGroupID = EoH_GroupHelper.GetGroupID(p);
            string otherGroupName = EoH_GroupHelper.GetGroupName(p);

            if (otherGroupID == s.CapturingGroupID)
            {
                captureGroupPresent = true;
                captureGroupInRadius++;
            }
            else if (otherGroupID != "")
            {
                enemyPresent = true;
                enemiesInRadius++;
            }

            if (EOH_CAPTURE_DEBUG)
                Print("[EoH_Capture][DEBUG] PlayerInZone town=" + s.TownName + " player=" + p.GetIdentity().GetName() + " group=" + otherGroupName + " groupID=" + otherGroupID + " dist=" + dist.ToString() + " playerPos=" + p.GetPosition().ToString() + " zonePos=" + pos.ToString());
        }

        if (EOH_CAPTURE_DEBUG)
            Print("[EoH_Capture][DEBUG] Presence town=" + s.TownName + " zonePos=" + pos.ToString() + " playersInRadius=" + playersInRadius.ToString() + " captureGroupInRadius=" + captureGroupInRadius.ToString() + " enemiesInRadius=" + enemiesInRadius.ToString() + " requiredGroupID=" + s.CapturingGroupID);

        bool wasContested = s.IsContested;
        bool wasPresent = s.CapturingGroupPresent;
        s.IsContested = enemyPresent;
        s.CapturingGroupPresent = captureGroupPresent;

        if (!s.CapturingGroupPresent)
        {
            if (!s.WasPausedNoPresence)
            {
                s.WasPausedNoPresence = true;
                EoH_TownMarkerManager.UpdatePausedMarker(s.TownName, s.CapturingGroupName);
                BroadcastCaptureMessage("TOWN CAPTURE PAUSED", s.TownName + " capture paused. Capturing group left the zone.");
                Print("[EoH_Capture] Paused no presence town=" + s.TownName + " group=" + s.CapturingGroupName);
            }
            return;
        }

        if (!wasPresent || s.WasPausedNoPresence)
        {
            s.WasPausedNoPresence = false;
            EoH_TownMarkerManager.UpdateCapturingMarker(s.TownName, s.CapturingGroupName);
            BroadcastCaptureMessage("TOWN CAPTURE RESUMED", s.TownName + " capture has resumed.");
            Print("[EoH_Capture] Resumed presence town=" + s.TownName + " group=" + s.CapturingGroupName);
        }

        if (s.IsContested)
        {
            EoH_TownMarkerManager.UpdateContestedMarker(s.TownName, s.CapturingGroupName);
            if (!wasContested)
                BroadcastCaptureMessage("TOWN CONTESTED", s.TownName + " is contested. Capture progress paused.");
        }
        else if (wasContested)
        {
            EoH_TownMarkerManager.UpdateCapturingMarker(s.TownName, s.CapturingGroupName);
            BroadcastCaptureMessage("TOWN CAPTURE RESUMED", s.TownName + " capture has resumed.");
        }
    }

    void UpdateCaptureProgressMarker(EoH_CaptureSession s, int percent)
    {
        EoH_MarkerData data = EoH_TownMarkerManager.BuildTownMarker(s.TownName, s.CapturingGroupName, EoH_MarkerState.CAPTURING, 1, ARGB(255, 255, 220, 80));
        data.Label = s.TownName + " Capturing " + percent.ToString() + "%";
        data.Icon = "Radio";
        data.Normalize();
        EoH_MarkerService.Broadcast(data);
    }

    void BroadcastCaptureMessage(string title, string msg)
    {
        EoH_Notifications.SendToAll(title, msg);
    }

    void CompleteCapture(EoH_CaptureSession s)
    {
        EoH_WorldStateManager.Get().SetTownOwner(s.TownName, s.CapturingGroupID, s.CapturingGroupName);
        EoH_TownRewardManager.SpawnCaptureReward(s.TownName, s.CapturingGroupName, GetTownPos(s.TownName));
        BroadcastCaptureMessage("TOWN CAPTURED", s.CapturingGroupName + " captured " + s.TownName + ".");
        EoH_TownMarkerManager.UpdateTownMarker(s.TownName, s.CapturingGroupName);
        Print("[EoH_Capture] Complete town=" + s.TownName + " owner=" + s.CapturingGroupName);
        m_Sessions.Remove(s.TownName);
    }
};
