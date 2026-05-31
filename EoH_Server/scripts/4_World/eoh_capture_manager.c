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

    float Distance2D(vector a, vector b)
    {
        float dx = a[0] - b[0];
        float dz = a[2] - b[2];
        return Math.Sqrt((dx * dx) + (dz * dz));
    }

    bool EoH_IsRelayType(string type)
    {
        return type == "EoH_RadioRelay" || type == "EoH_CaptureRelay_Base" || type.Contains("EoH_RadioRelay") || type.Contains("EoH_CaptureRelay");
    }

    vector GetRelayConfigPos(string town)
    {
        EoH_RelayConfig relayCfg = GetEoHRelayConfig();
        if (!relayCfg || !relayCfg.Relays)
            return "0 0 0".ToVector();

        foreach (EoH_RelayLocation relay : relayCfg.Relays)
        {
            if (!relay || !relay.Enabled)
                continue;

            if (relay.TownName == town)
                return relay.GetPosition();
        }

        return "0 0 0".ToVector();
    }

    vector GetLiveRelayPosNear(vector expected, string town)
    {
        vector fallback = "0 0 0".ToVector();
        if (expected == fallback)
            return fallback;

        array<Object> objects = new array<Object>();
        vector searchPos = expected;
        GetGame().GetObjectsAtPosition3D(searchPos, 75.0, objects, null);

        Object bestObj;
        float bestDist = 999999.0;

        foreach (Object obj : objects)
        {
            if (!obj || !EoH_IsRelayType(obj.GetType()))
                continue;

            float dist = Distance2D(obj.GetPosition(), expected);
            if (dist < bestDist)
            {
                bestDist = dist;
                bestObj = obj;
            }
        }

        if (bestObj)
        {
            if (EOH_CAPTURE_DEBUG)
                Print("[EoH_Capture][DEBUG] Using live relay near config town=" + town + " relayPos=" + bestObj.GetPosition().ToString() + " dist2DFromConfig=" + bestDist.ToString());

            return bestObj.GetPosition();
        }

        return fallback;
    }

    vector GetLiveRelayPos(string town)
    {
        vector cfgPos = GetRelayConfigPos(town);
        vector livePos = GetLiveRelayPosNear(cfgPos, town);
        if (livePos != "0 0 0".ToVector())
            return livePos;

        if (m_Towns.Contains(town))
            return GetLiveRelayPosNear(m_Towns.Get(town), town);

        return "0 0 0".ToVector();
    }

    vector GetTownPos(string town)
    {
        vector cfgPos = GetRelayConfigPos(town);
        if (cfgPos != "0 0 0".ToVector())
        {
            vector livePos = GetLiveRelayPosNear(cfgPos, town);
            if (livePos != "0 0 0".ToVector())
                return livePos;

            if (EOH_CAPTURE_DEBUG)
                Print("[EoH_Capture][DEBUG] Using exact relay config position for town=" + town + " cfgPos=" + cfgPos.ToString());

            return cfgPos;
        }

        vector legacyLivePos = GetLiveRelayPos(town);
        if (legacyLivePos != "0 0 0".ToVector())
            return legacyLivePos;

        if (m_Towns.Contains(town))
            return m_Towns.Get(town);

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
        vector pos = GetTownPos(town);
        if (pos == "0 0 0".ToVector())
            return null;

        EoH_CaptureTownConfig cfg = new EoH_CaptureTownConfig();
        cfg.Name = town;
        cfg.RelayPosition.Clear();
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
            EoH_LiveAdvisorActivity.LogActivity("town_capture", "start_blocked_already_active town=" + town);
            return;
        }

        if (!player)
            return;

        vector townPos = GetTownPos(town);
        float dist2D = Distance2D(player.GetPosition(), townPos);
        float dist3D = vector.Distance(player.GetPosition(), townPos);

        if (dist2D > CAPTURE_RADIUS)
        {
            EoH_Notifications.SendToPlayer(player, "TOWN CAPTURE FAILED", "You must be within " + CAPTURE_RADIUS.ToString() + "m of the radio relay to start capturing " + town + ".");
            Print("[EoH_Capture] Blocked start outside relay radius town=" + town + " player=" + player.GetIdentity().GetName() + " dist2D=" + dist2D.ToString() + " dist3D=" + dist3D.ToString() + " playerPos=" + player.GetPosition().ToString() + " relayPos=" + townPos.ToString());
            EoH_LiveAdvisorActivity.LogActivity("town_capture", "start_blocked_radius town=" + town + " dist2D=" + dist2D.ToString());
            return;
        }

        string groupID = EoH_GroupHelper.GetGroupID(player);
        if (groupID == "")
        {
            EoH_Notifications.SendToPlayer(player, "TOWN CAPTURE FAILED", "You must be in a group to capture a town.");
            Print("[EoH_Capture] Blocked start no group town=" + town + " player=" + player.GetIdentity().GetName());
            EoH_LiveAdvisorActivity.LogActivity("town_capture", "start_blocked_no_group town=" + town);
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
        SendTownCaptureProgress(s, "RPC_Show", "Capturing town relay...", false, false);
        BroadcastCaptureMessage("TOWN CAPTURE STARTED", s.CapturingGroupName + " started capturing " + town + ". Hold the radio relay for 10 minutes.");
        Print("[EoH_Capture] Started capture town=" + town + " group=" + s.CapturingGroupName + " playerPos=" + player.GetPosition().ToString() + " relayPos=" + townPos.ToString() + " dist2D=" + dist2D.ToString());
        EoH_LiveAdvisorActivity.LogActivity("town_capture", "capture_started town=" + town + " group=" + s.CapturingGroupName + " dist2D=" + dist2D.ToString());
    }

    void Tick()
    {
        int tickStart = GetGame().GetTime();
        int now = tickStart;
        int processedSessions = 0;

        if (EOH_CAPTURE_DEBUG && now - m_LastDebugTick >= 30000)
        {
            m_LastDebugTick = now;
            Print("[EoH_Capture][DEBUG] Tick sessions=" + m_Sessions.Count().ToString() + " towns=" + m_Towns.Count().ToString());
        }

        foreach (string town, EoH_CaptureSession s : m_Sessions)
        {
            processedSessions++;
            int delta = now - s.LastTick;
            s.LastTick = now;

            int presenceStart = GetGame().GetTime();
            UpdatePresence(s);
            int presenceElapsed = GetGame().GetTime() - presenceStart;
            EoH_LiveAdvisorActivity.LogActivity("town_capture", "presence_checked town=" + s.TownName + " elapsedMs=" + presenceElapsed.ToString() + " present=" + s.CapturingGroupPresent.ToString() + " contested=" + s.IsContested.ToString());

            if (EOH_CAPTURE_DEBUG)
                Print("[EoH_Capture][DEBUG] Session town=" + s.TownName + " group=" + s.CapturingGroupName + " present=" + s.CapturingGroupPresent.ToString() + " contested=" + s.IsContested.ToString() + " progressMs=" + s.Progress.ToString() + " delta=" + delta.ToString());

            if (!s.CapturingGroupPresent)
            {
                SendTownCaptureProgress(s, "RPC_Update", "Capture paused. Group left relay zone.", false, false);
                continue;
            }

            if (!s.IsContested)
            {
                s.Progress += delta;

                int percent = Math.Clamp(Math.Floor((s.Progress / CAPTURE_DURATION_MS) * 100.0), 0, 100);
                int bucket = Math.Floor(percent / 10) * 10;

                SendTownCaptureProgress(s, "RPC_Update", "Capturing town relay...", false, false);

                if (bucket >= s.LastNotifyPercent + 10 || percent >= 100)
                {
                    s.LastNotifyPercent = bucket;
                    UpdateCaptureProgressMarker(s, percent);
                    BroadcastCaptureMessage("TOWN CAPTURE", s.TownName + " capture progress: " + percent.ToString() + "%");
                    Print("[EoH_Capture] Progress town=" + s.TownName + " group=" + s.CapturingGroupName + " percent=" + percent.ToString());
                    EoH_LiveAdvisorActivity.LogActivity("town_capture", "progress town=" + s.TownName + " percent=" + percent.ToString());
                }

                if (s.Progress > CAPTURE_DURATION_MS)
                    CompleteCapture(s);
            }
            else
            {
                SendTownCaptureProgress(s, "RPC_Update", "Signal contested. Capture paused.", false, false);
            }
        }

        int elapsed = GetGame().GetTime() - tickStart;
        EoH_LiveAdvisorActivity.LogActivity("town_capture", "tick_end sessions=" + m_Sessions.Count().ToString() + " processed=" + processedSessions.ToString() + " elapsedMs=" + elapsed.ToString());
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

            float dist = Distance2D(p.GetPosition(), pos);
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
                Print("[EoH_Capture][DEBUG] PlayerInRelayZone town=" + s.TownName + " player=" + p.GetIdentity().GetName() + " group=" + otherGroupName + " groupID=" + otherGroupID + " dist2D=" + dist.ToString() + " playerPos=" + p.GetPosition().ToString() + " relayPos=" + pos.ToString());
        }

        if (EOH_CAPTURE_DEBUG)
            Print("[EoH_Capture][DEBUG] Presence town=" + s.TownName + " relayPos=" + pos.ToString() + " playersInRadius=" + playersInRadius.ToString() + " captureGroupInRadius=" + captureGroupInRadius.ToString() + " enemiesInRadius=" + enemiesInRadius.ToString() + " requiredGroupID=" + s.CapturingGroupID);

        EoH_LiveAdvisorActivity.LogActivity("town_capture", "presence town=" + s.TownName + " playersInRadius=" + playersInRadius.ToString() + " captureGroupInRadius=" + captureGroupInRadius.ToString() + " enemiesInRadius=" + enemiesInRadius.ToString());

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
                BroadcastCaptureMessage("TOWN CAPTURE PAUSED", s.TownName + " capture paused. Capturing group left the radio relay.");
                SendTownCaptureProgress(s, "RPC_Update", "Capture paused. Group left relay zone.", false, false);
                Print("[EoH_Capture] Paused no presence town=" + s.TownName + " group=" + s.CapturingGroupName);
                EoH_LiveAdvisorActivity.LogActivity("town_capture", "paused_no_presence town=" + s.TownName + " group=" + s.CapturingGroupName);
            }
            return;
        }

        if (!wasPresent || s.WasPausedNoPresence)
        {
            s.WasPausedNoPresence = false;
            EoH_TownMarkerManager.UpdateCapturingMarker(s.TownName, s.CapturingGroupName);
            BroadcastCaptureMessage("TOWN CAPTURE RESUMED", s.TownName + " capture has resumed.");
            SendTownCaptureProgress(s, "RPC_Update", "Capturing town relay...", false, false);
            Print("[EoH_Capture] Resumed presence town=" + s.TownName + " group=" + s.CapturingGroupName);
            EoH_LiveAdvisorActivity.LogActivity("town_capture", "resumed_presence town=" + s.TownName + " group=" + s.CapturingGroupName);
        }

        if (s.IsContested)
        {
            EoH_TownMarkerManager.UpdateContestedMarker(s.TownName, s.CapturingGroupName);
            if (!wasContested)
            {
                BroadcastCaptureMessage("TOWN CONTESTED", s.TownName + " is contested. Capture progress paused.");
                SendTownCaptureProgress(s, "RPC_Update", "Signal contested. Capture paused.", false, false);
                EoH_LiveAdvisorActivity.LogActivity("town_capture", "contested town=" + s.TownName + " group=" + s.CapturingGroupName);
            }
        }
        else if (wasContested)
        {
            EoH_TownMarkerManager.UpdateCapturingMarker(s.TownName, s.CapturingGroupName);
            BroadcastCaptureMessage("TOWN CAPTURE RESUMED", s.TownName + " capture has resumed.");
            SendTownCaptureProgress(s, "RPC_Update", "Capturing town relay...", false, false);
            EoH_LiveAdvisorActivity.LogActivity("town_capture", "resumed_after_contest town=" + s.TownName + " group=" + s.CapturingGroupName);
        }
    }

    void UpdateCaptureProgressMarker(EoH_CaptureSession s, int percent)
    {
        EoH_MarkerData data = EoH_TownMarkerManager.BuildTownMarker(s.TownName, s.CapturingGroupName, EoH_MarkerState.CAPTURING, 1, ARGB(255, 255, 220, 80));
        data.Label = s.TownName + " " + EoH_TownMarkerManager.GetGroupTag(s.CapturingGroupName) + " Capturing " + percent.ToString() + "%";
        data.Icon = "Radio";
        data.Position = GetTownPos(s.TownName);
        data.Normalize();
        EoH_MarkerService.Broadcast(data);
    }

    EoH_ObjectiveProgressData BuildTownCaptureProgressData(EoH_CaptureSession s, string status, bool complete, bool failed)
    {
        EoH_ObjectiveProgressData data = new EoH_ObjectiveProgressData();
        if (!s)
            return data;

        data.Channel = "town_capture_" + s.TownName;
        data.Title = "RELAY CONTROL: " + s.TownName;
        data.Status = status;
        data.Progress01 = s.Progress / CAPTURE_DURATION_MS;
        data.LeftLabel = "Group: " + s.CapturingGroupName;

        if (complete)
            data.RightLabel = "Complete";
        else if (s.IsContested)
            data.RightLabel = "Contested: Yes";
        else if (!s.CapturingGroupPresent)
            data.RightLabel = "Holding: No";
        else
            data.RightLabel = "Contested: No";

        data.Complete = complete;
        data.Failed = failed;
        data.Contested = s.IsContested;
        data.Normalize();
        return data;
    }

    void SendTownCaptureProgress(EoH_CaptureSession s, string rpcName, string status, bool complete, bool failed)
    {
        if (!s || s.CapturingGroupID == "")
            return;

        EoH_ObjectiveProgressData data = BuildTownCaptureProgressData(s, status, complete, failed);
        Param1<EoH_ObjectiveProgressData> param = new Param1<EoH_ObjectiveProgressData>(data);

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            if (EoH_GroupHelper.GetGroupID(player) != s.CapturingGroupID)
                continue;

            GetRPCManager().SendRPC("EoH_ObjectiveProgress", rpcName, param, true, player.GetIdentity());
        }
    }

    void HideTownCaptureProgress(EoH_CaptureSession s)
    {
        if (!s || s.CapturingGroupID == "")
            return;

        Param1<string> param = new Param1<string>("town_capture_" + s.TownName);
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            if (EoH_GroupHelper.GetGroupID(player) != s.CapturingGroupID)
                continue;

            GetRPCManager().SendRPC("EoH_ObjectiveProgress", "RPC_Hide", param, true, player.GetIdentity());
        }
    }

    void BroadcastCaptureMessage(string title, string msg)
    {
        EoH_Notifications.SendToAll(title, msg);
    }

    void CompleteCapture(EoH_CaptureSession s)
    {
        vector relayPos = GetTownPos(s.TownName);
        EoH_WorldStateManager.Get().SetTownOwner(s.TownName, s.CapturingGroupID, s.CapturingGroupName);
        EoH_TownRewardManager.SpawnCaptureReward(s.TownName, s.CapturingGroupName, relayPos);
        SendTownCaptureProgress(s, "RPC_Update", "Town captured.", true, false);
        BroadcastCaptureMessage("TOWN CAPTURED", s.CapturingGroupName + " captured " + s.TownName + ".");
        EoH_TownMarkerManager.UpdateTownMarker(s.TownName, s.CapturingGroupName);
        HideTownCaptureProgress(s);
        Print("[EoH_Capture] Complete town=" + s.TownName + " owner=" + s.CapturingGroupName + " relayPos=" + relayPos.ToString());
        EoH_LiveAdvisorActivity.LogActivity("town_capture", "capture_complete town=" + s.TownName + " group=" + s.CapturingGroupName);
        m_Sessions.Remove(s.TownName);
    }
};