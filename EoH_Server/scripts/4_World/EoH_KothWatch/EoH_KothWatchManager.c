class EoH_KothWatchManager
{
    protected static ref EoH_KothWatchManager s_Instance;
    protected ref EoH_KothWatchConfig m_Config;
    protected float m_LastPlayerCheckTime;
    protected float m_LastHeartbeatTime;

    protected const string CONFIG_DIR = "$profile:EoH";
    protected const string CONFIG_PATH = "$profile:EoH/KothWatchConfig.json";
    protected const float HEARTBEAT_SECONDS = 120.0;

    void EoH_KothWatchManager()
    {
        m_LastPlayerCheckTime = 0;
        m_LastHeartbeatTime = 0;
        LoadConfig();
    }

    static EoH_KothWatchManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_KothWatchManager();

        return s_Instance;
    }

    void LoadConfig()
    {
        EnsureConfigDir();
        m_Config = new EoH_KothWatchConfig();

        if (!FileExist(CONFIG_PATH))
        {
            m_Config.Defaults();
            JsonFileLoader<EoH_KothWatchConfig>.JsonSaveFile(CONFIG_PATH, m_Config);
            EoH_LiveAdvisorActivity.LogActivity("koth_watch", "created_default_config path=" + CONFIG_PATH);
        }
        else
        {
            JsonFileLoader<EoH_KothWatchConfig>.JsonLoadFile(CONFIG_PATH, m_Config);
            NormalizeConfig();
            EoH_LiveAdvisorActivity.LogActivity("koth_watch", "loaded_config enabled=" + m_Config.Enabled.ToString() + " zones=" + m_Config.Zones.Count().ToString() + " warnOnly=" + m_Config.WarnOnly.ToString());
        }

        LogSafeModeState("startup");
    }

    void NormalizeConfig()
    {
        if (!m_Config)
            return;

        if (!m_Config.Zones)
            m_Config.Zones = new array<ref EoH_KothWatchZone>();

        if (m_Config.Zones.Count() == 0)
            m_Config.AddDefaultElektroZone();

        if (m_Config.PlayerCheckSeconds <= 0)
            m_Config.PlayerCheckSeconds = 30.0;

        if (m_Config.DisconnectRiskRadius <= 0)
            m_Config.DisconnectRiskRadius = 750.0;

        if (m_Config.SafeModeRadius <= 0)
            m_Config.SafeModeRadius = 750.0;
    }

    void EnsureConfigDir()
    {
        if (!FileExist(CONFIG_DIR))
            MakeDirectory(CONFIG_DIR);
    }

    bool IsEnabled()
    {
        return m_Config && m_Config.Enabled;
    }

    void Tick()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!IsEnabled())
            return;

        float now = GetGame().GetTime() / 1000.0;
        WriteHeartbeat(now);

        if (m_LastPlayerCheckTime > 0 && now - m_LastPlayerCheckTime < m_Config.PlayerCheckSeconds)
            return;

        m_LastPlayerCheckTime = now;
        CheckPlayersNearZones();
    }

    void WriteHeartbeat(float now)
    {
        if (!m_Config)
            return;

        if (m_LastHeartbeatTime > 0 && now - m_LastHeartbeatTime < HEARTBEAT_SECONDS)
            return;

        m_LastHeartbeatTime = now;

        int playersOnline = GetPlayerCount();
        int zones = 0;
        if (m_Config.Zones)
            zones = m_Config.Zones.Count();

        EoH_LiveAdvisorActivity.LogActivity("koth_watch", "heartbeat enabled=" + m_Config.Enabled.ToString() + " zones=" + zones.ToString() + " players=" + playersOnline.ToString() + " checkSeconds=" + m_Config.PlayerCheckSeconds.ToString());
        LogSafeModeState("heartbeat");
    }

    void LogSafeModeState(string source)
    {
        if (!m_Config)
            return;

        EoH_LiveAdvisorActivity.LogActivity("koth_watch", "safe_mode_state source=" + source + " enabled=" + m_Config.EnableKothSafeMode.ToString() + " disableTownAI=" + m_Config.DisableTownAINearKoth.ToString() + " suppressMarkers=" + m_Config.SuppressEoHMarkersNearKoth.ToString() + " radius=" + m_Config.SafeModeRadius.ToString());
    }

    int GetPlayerCount()
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        if (!players)
            return 0;

        return players.Count();
    }

    void CheckPlayersNearZones()
    {
        if (!m_Config || !m_Config.LogPlayerProximity || !m_Config.Zones)
            return;

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            EoH_KothWatchZone zone = FindNearestZone(player.GetPosition());
            if (!zone)
                continue;

            float dist = Distance2D(player.GetPosition(), zone.GetPosition());
            if (dist > zone.Radius)
                continue;

            float relayDist = Distance2D(player.GetPosition(), zone.GetNearestTownRelayPosition());
            EoH_LiveAdvisorActivity.LogActivity("koth_watch", "player_near_zone name=" + player.GetIdentity().GetName() + " zone=" + zone.Name + " dist=" + dist.ToString() + " nearestTown=" + zone.NearestTown + " relayDist=" + relayDist.ToString() + " pos=" + player.GetPosition().ToString());

            if (relayDist <= m_Config.DisconnectRiskRadius)
            {
                EoH_LiveAdvisorActivity.LogActivity("koth_watch", "overlap_warning zone=" + zone.Name + " nearestTown=" + zone.NearestTown + " player=" + player.GetIdentity().GetName() + " relayDist=" + relayDist.ToString());
            }
        }
    }

    void OnClientReady(PlayerBase player, PlayerIdentity identity)
    {
        if (!IsEnabled() || !m_Config.LogConnectsNearZones)
            return;

        string name = "unknown";
        vector pos = "0 0 0".ToVector();

        if (identity)
            name = identity.GetName();

        if (player)
            pos = player.GetPosition();

        LogPositionEvent("client_ready", name, pos);
    }

    void OnDisconnect(PlayerBase player, PlayerIdentity identity, string uid)
    {
        if (!IsEnabled() || !m_Config.LogDisconnectsNearZones)
            return;

        string name = "unknown";
        vector pos = "0 0 0".ToVector();

        if (identity)
            name = identity.GetName();

        if (player)
            pos = player.GetPosition();

        LogPositionEvent("disconnect", name, pos);
    }

    void LogPositionEvent(string eventName, string playerName, vector pos)
    {
        EoH_KothWatchZone zone = FindNearestZone(pos);
        if (!zone)
            return;

        float dist = Distance2D(pos, zone.GetPosition());
        float relayDist = Distance2D(pos, zone.GetNearestTownRelayPosition());

        if (dist <= zone.Radius || relayDist <= m_Config.DisconnectRiskRadius)
        {
            EoH_LiveAdvisorActivity.LogActivity("koth_watch", eventName + "_near_zone name=" + playerName + " zone=" + zone.Name + " dist=" + dist.ToString() + " nearestTown=" + zone.NearestTown + " relayDist=" + relayDist.ToString() + " pos=" + pos.ToString());
        }
    }

    EoH_KothWatchZone FindNearestZone(vector pos)
    {
        if (!m_Config || !m_Config.Zones || m_Config.Zones.Count() == 0)
            return null;

        EoH_KothWatchZone nearest;
        float bestDist = 999999.0;

        foreach (EoH_KothWatchZone zone : m_Config.Zones)
        {
            if (!zone)
                continue;

            float dist = Distance2D(pos, zone.GetPosition());
            if (dist < bestDist)
            {
                bestDist = dist;
                nearest = zone;
            }
        }

        return nearest;
    }

    float Distance2D(vector a, vector b)
    {
        float dx = a[0] - b[0];
        float dz = a[2] - b[2];
        return Math.Sqrt((dx * dx) + (dz * dz));
    }
}
