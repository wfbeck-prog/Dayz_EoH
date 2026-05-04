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
    int LastUIPercent;
    int LastContestAlertTime;

    void EoH_CaptureSession()
    {
        TownName = "";
        AttackingGroupID = "";
        AttackingGroupName = "";
        Progress = 0;
        LastTickTime = 0;
        IsContested = false;
        AttackersNearby = 0;
        EnemiesNearby = 0;
        LastUIPercent = -5;
        LastContestAlertTime = 0;
    }
};

class EoH_CaptureManager
{
    static ref EoH_CaptureManager s_Instance;

    ref map<string, ref EoH_CaptureSession> m_ActiveCaptures;
    ref EoH_CaptureConfig m_Config;

    protected static const string EOH_CAPTURE_CONFIG_FILE = "$profile:EoH\\WorldState\\EoH_CaptureConfig.json";

    void EoH_CaptureManager()
    {
        m_ActiveCaptures = new map<string, ref EoH_CaptureSession>();
        LoadConfig();
    }

    static EoH_CaptureManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_CaptureManager();

        return s_Instance;
    }

    void LoadConfig()
    {
        MakeDirectory("$profile:EoH");
        MakeDirectory("$profile:EoH\\WorldState");

        if (FileExist(EOH_CAPTURE_CONFIG_FILE))
        {
            JsonFileLoader<EoH_CaptureConfig>.JsonLoadFile(EOH_CAPTURE_CONFIG_FILE, m_Config);
            if (!m_Config)
                CreateDefaultConfig();
        }
        else
        {
            CreateDefaultConfig();
        }

        EnsureConfigValid();
        JsonFileLoader<EoH_CaptureConfig>.JsonSaveFile(EOH_CAPTURE_CONFIG_FILE, m_Config);
    }

    protected void CreateDefaultConfig()
    {
        m_Config = new EoH_CaptureConfig();

        AddTownConfig("Pustoshka", 1, 180, 80, 900, "3060 0 7870");
        AddTownConfig("Mogilevka", 1, 200, 80, 900, "7600 0 5100");
        AddTownConfig("Guglovo", 1, 180, 80, 900, "8500 0 6600");
        AddTownConfig("Tulga", 1, 160, 70, 720, "12800 0 9400");
        AddTownConfig("Nadezhdino", 1, 170, 70, 720, "5900 0 4700");
        AddTownConfig("Kamenka", 1, 200, 80, 900, "1900 0 2200");

        AddTownConfig("Vybor", 2, 250, 90, 1200, "3700 0 8800");
        AddTownConfig("Stary Sobor", 2, 260, 90, 1320, "6200 0 7800");
        AddTownConfig("Novy Sobor", 2, 240, 90, 1200, "7000 0 7600");
        AddTownConfig("Zelenogorsk", 2, 280, 100, 1500, "2800 0 5300");
        AddTownConfig("Staroye", 2, 220, 85, 1080, "10200 0 5400");
        AddTownConfig("Polana", 2, 220, 85, 1080, "10700 0 8100");

        AddTownConfig("Elektro", 3, 400, 120, 1800, "10500 0 2250");
        AddTownConfig("Chernogorsk", 3, 420, 120, 1800, "6600 0 2600");
        AddTownConfig("Berezino", 3, 400, 120, 1800, "12000 0 9000");

        AddTownConfig("NWAF", 4, 500, 140, 2100, "4800 0 9600");
        AddTownConfig("Tisy", 4, 450, 140, 2100, "1700 0 13800");
        AddTownConfig("Pavlovo Military", 4, 300, 110, 1500, "2100 0 3300");
    }

    protected void EnsureConfigValid()
    {
        if (!m_Config)
            CreateDefaultConfig();

        if (!m_Config.Towns)
            m_Config.Towns = new array<ref EoH_CaptureTownConfig>();

        if (m_Config.TickSeconds <= 0)
            m_Config.TickSeconds = 10;

        if (m_Config.Towns.Count() == 0)
            CreateDefaultConfig();
    }

    protected void AddTownConfig(string name, int tier, float radius, float contestRadius, int captureSeconds, vector relayPos)
    {
        EoH_CaptureTownConfig town = new EoH_CaptureTownConfig();
        town.Name = name;
        town.Tier = tier;
        town.Radius = radius;
        town.ContestRadius = contestRadius;
        town.CaptureTimeSeconds = captureSeconds;
        town.RequiredGroupMembers = 2;
        town.RelayPosition.Insert(relayPos[0]);
        town.RelayPosition.Insert(relayPos[1]);
        town.RelayPosition.Insert(relayPos[2]);
        m_Config.Towns.Insert(town);
    }

    array<string> GetAllTownNames()
    {
        array<string> towns = new array<string>();

        if (!m_Config || !m_Config.Towns)
            return towns;

        foreach (EoH_CaptureTownConfig town : m_Config.Towns)
        {
            if (town && town.Name != "")
                towns.Insert(town.Name);
        }

        return towns;
    }

    string GetTownOwner(string townName)
    {
        EoH_WorldStateTownState state = EoH_WorldStateManager.Get().GetTownState(townName);
        if (!state || state.OwnerGroupID == "")
            return "";

        return state.OwnerGroupName;
    }

    EoH_CaptureTownConfig GetTownConfig(string townName)
    {
        if (!m_Config || !m_Config.Towns)
            return null;

        foreach (EoH_CaptureTownConfig town : m_Config.Towns)
        {
            if (town && town.Name == townName)
                return town;
        }

        return null;
    }

    void StartCapture(string townName, PlayerBase player)
    {
        if (!m_Config || !m_Config.Enabled)
            return;

        if (!player || !player.GetIdentity())
            return;

        EoH_CaptureTownConfig townCfg = GetTownConfig(townName);
        if (!townCfg)
        {
            Print("[EoH_Capture] Cannot start capture. Missing town config: " + townName);
            return;
        }

        string groupID = GetPlayerGroupID(player);
        string groupName = GetPlayerGroupName(player);

        if (groupID == "")
        {
            EoH_CaptureUI.Send(player, "Capture denied: you must be in an Expansion group.");
            Print("[EoH_Capture] Capture denied. Player is not in an Expansion group.");
            return;
        }

        if (!m_ActiveCaptures.Contains(townName))
        {
            EoH_CaptureSession session = new EoH_CaptureSession();
            session.TownName = townName;
            session.AttackingGroupID = groupID;
            session.AttackingGroupName = groupName;
            session.LastTickTime = GetGame().GetTime();

            m_ActiveCaptures.Insert(townName, session);

            SendMarkerToNearby(townCfg, townName, ARGB(255, 255, 220, 80), false);
            NotifyNearbyPlayers(townName, "Capture started at " + townName + " by " + groupName);
            Print("[EoH_Capture] Capture started: " + townName + " by " + groupName);
        }
        else
        {
            EoH_CaptureUI.Send(player, "Capture already active: " + townName);
            Print("[EoH_Capture] Capture already active: " + townName);
        }
    }

    void Tick()
    {
        if (!m_Config || !m_Config.Enabled)
            return;

        int now = GetGame().GetTime();
        ref array<string> completed = new array<string>();
        ref array<string> keys = m_ActiveCaptures.GetKeyArray();

        foreach (string townName : keys)
        {
            EoH_CaptureSession session = m_ActiveCaptures.Get(townName);
            if (!session)
                continue;

            EoH_CaptureTownConfig townCfg = GetTownConfig(townName);
            if (!townCfg)
                continue;

            int delta = now - session.LastTickTime;
            session.LastTickTime = now;

            UpdatePresence(session, townCfg);

            if (CanProgress(session, townCfg))
            {
                session.Progress += delta;
                SendMarkerToNearby(townCfg, session.TownName, ARGB(255, 255, 220, 80), false);

                float percent = (session.Progress / (townCfg.CaptureTimeSeconds * 1000.0)) * 100.0;
                if (percent > 100.0)
                    percent = 100.0;

                int roundedPercent = Math.Round(percent);
                if (roundedPercent >= session.LastUIPercent + 5)
                {
                    session.LastUIPercent = roundedPercent;
                    NotifyNearbyPlayers(session.TownName, "Capturing " + session.TownName + ": " + roundedPercent.ToString() + "%");
                }
            }
            else
            {
                if (session.IsContested)
                {
                    if (now - session.LastContestAlertTime > 30000)
                    {
                        session.LastContestAlertTime = now;
                        NotifyNearbyPlayers(session.TownName, "CONTESTED: " + session.TownName + " (" + session.EnemiesNearby.ToString() + " enemies nearby)");
                    }
                }
                else
                {
                    if (session.Progress > 0)
                    {
                        session.Progress -= delta * 0.5;
                        if (session.Progress < 0)
                            session.Progress = 0;
                    }
                }
            }

            float required = townCfg.CaptureTimeSeconds * 1000.0;
            if (session.Progress >= required)
                completed.Insert(townName);
        }

        foreach (string completeTown : completed)
        {
            EoH_CaptureSession completeSession = m_ActiveCaptures.Get(completeTown);
            if (completeSession)
                CompleteCapture(completeSession);
        }
    }

    protected void UpdatePresence(EoH_CaptureSession session, EoH_CaptureTownConfig townCfg)
    {
        bool wasContested = session.IsContested;
        session.AttackersNearby = 0;
        session.EnemiesNearby = 0;
        session.IsContested = false;

        vector center = townCfg.GetRelayVector();
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity() || !player.IsAlive())
                continue;

            float dist = vector.Distance(player.GetPosition(), center);
            if (dist > townCfg.Radius)
                continue;

            string playerGroupID = GetPlayerGroupID(player);
            if (playerGroupID == "")
                continue;

            if (playerGroupID == session.AttackingGroupID)
                session.AttackersNearby++;
            else if (dist <= townCfg.ContestRadius)
                session.EnemiesNearby++;
        }

        if (m_Config.PauseWhenContested && session.EnemiesNearby > 0)
            session.IsContested = true;

        if (session.IsContested)
        {
            SendContestedMarkerToNearby(townCfg, session.TownName);
        }
        else if (wasContested)
        {
            SendMarkerToNearby(townCfg, session.TownName, ARGB(120, 150, 150, 150), false);
        }
    }

    protected bool CanProgress(EoH_CaptureSession session, EoH_CaptureTownConfig townCfg)
    {
        if (session.IsContested)
            return false;

        int required = townCfg.RequiredGroupMembers;

        if (m_Config.AllowSoloCaptureFallback && required < 1)
            required = 1;

        if (session.AttackersNearby < required)
            return false;

        return true;
    }

    protected void SendMarkerToNearby(EoH_CaptureTownConfig cfg, string townName, int color, bool pulse)
    {
        vector pos = cfg.GetRelayVector();
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            if (vector.Distance(player.GetPosition(), pos) <= cfg.Radius * 1.5)
            {
                GetGame().RPCSingleParam(
                    player,
                    777001,
                    new Param3<vector, string, int>(pos, townName, color),
                    true,
                    player.GetIdentity()
                );
            }
        }
    }

    protected void SendContestedMarkerToNearby(EoH_CaptureTownConfig cfg, string townName)
    {
        vector pos = cfg.GetRelayVector();
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            if (vector.Distance(player.GetPosition(), pos) <= cfg.Radius * 1.5)
            {
                GetGame().RPCSingleParam(
                    player,
                    777003,
                    new Param2<vector, string>(pos, townName),
                    true,
                    player.GetIdentity()
                );
            }
        }
    }

    protected void NotifyNearbyPlayers(string townName, string msg)
    {
        EoH_CaptureTownConfig cfg = GetTownConfig(townName);
        if (!cfg)
            return;

        vector center = cfg.GetRelayVector();
        float notifyRadius = cfg.Radius * 1.5;

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            if (vector.Distance(player.GetPosition(), center) <= notifyRadius)
                EoH_CaptureUI.Send(player, msg);
        }
    }

    protected string GetPlayerGroupID(PlayerBase player)
    {
        return EoH_GroupHelper.GetGroupID(player);
    }

    protected string GetPlayerGroupName(PlayerBase player)
    {
        return EoH_GroupHelper.GetGroupName(player);
    }

    void CompleteCapture(EoH_CaptureSession session)
    {
        EoH_WorldStateManager.Get().SetTownOwner(session.TownName, session.AttackingGroupID, session.AttackingGroupName);

        EoH_CaptureTownConfig cfg = GetTownConfig(session.TownName);
        if (cfg)
            SendMarkerToNearby(cfg, session.TownName, ARGB(255, 80, 255, 120), false);

        NotifyNearbyPlayers(session.TownName, session.TownName + " captured by " + session.AttackingGroupName);
        EoH_CaptureUI.Broadcast(session.TownName + " has been captured by " + session.AttackingGroupName);

        Print("[EoH_Capture] Capture complete: " + session.TownName);

        m_ActiveCaptures.Remove(session.TownName);
    }
};
