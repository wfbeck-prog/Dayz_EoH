modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();

        EoH_LiveAdvisorLogger.Init();
        EoH_LiveAdvisorLogger.Log("MISSION_INIT", "MissionServer initialized Live Advisor", "info", "MissionServer");
        EoH_KothWatchManager.Get();
        EoH_AdvisorRecommendationManager.Get();
        EoH_WeeklyEventPersistenceManager.Get();
        EoH_StartLiveAdvisorHeartbeat();
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        EoH_LiveAdvisorPerformance.CheckServerPerformance(timeslice);
        EoH_KothWatchManager.Get().Tick();
        EoH_AdvisorRecommendationManager.Get().Tick();
    }

    override void OnClientReadyEvent(PlayerIdentity identity, PlayerBase player)
    {
        super.OnClientReadyEvent(identity, player);

        string playerName = "unknown";
        string playerId = "unknown";
        vector playerPos = "0 0 0";

        if (identity)
        {
            playerName = identity.GetName();
            playerId = identity.GetId();
        }

        if (player)
        {
            playerPos = player.GetPosition();
        }

        EoH_LiveAdvisorActivity.LogActivity("player_session", "client_ready name=" + playerName + " id=" + playerId + " pos=" + playerPos.ToString() + " players=" + EoH_GetLiveAdvisorPlayerCount().ToString());
        EoH_KothWatchManager.Get().OnClientReady(player, identity);
    }

    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);

        string playerName = "unknown";
        string playerId = "unknown";
        vector playerPos = "0 0 0";

        if (identity)
        {
            playerName = identity.GetName();
            playerId = identity.GetId();
        }

        if (player)
        {
            playerPos = player.GetPosition();
        }

        EoH_LiveAdvisorActivity.LogActivity("player_session", "connect name=" + playerName + " id=" + playerId + " pos=" + playerPos.ToString() + " players=" + EoH_GetLiveAdvisorPlayerCount().ToString());
    }

    override void PlayerDisconnected(PlayerBase player, PlayerIdentity identity, string uid)
    {
        super.PlayerDisconnected(player, identity, uid);

        string playerName = "unknown";
        string playerId = uid;
        vector playerPos = "0 0 0";

        if (identity)
        {
            playerName = identity.GetName();
            playerId = identity.GetId();
        }

        if (player)
        {
            playerPos = player.GetPosition();
        }

        EoH_LiveAdvisorActivity.LogActivity("player_session", "disconnect name=" + playerName + " id=" + playerId + " pos=" + playerPos.ToString() + " players=" + EoH_GetLiveAdvisorPlayerCount().ToString());
        EoH_KothWatchManager.Get().OnDisconnect(player, identity, uid);
    }

    int EoH_GetLiveAdvisorPlayerCount()
    {
        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);

        if (!players)
            return 0;

        return players.Count();
    }

    void EoH_StartLiveAdvisorHeartbeat()
    {
        if (!EoH_LiveAdvisorLogger.m_Config || !EoH_LiveAdvisorLogger.m_Config.Enabled)
        {
            return;
        }

        int heartbeatSeconds = EoH_LiveAdvisorLogger.m_Config.HeartbeatSeconds;

        if (heartbeatSeconds < 60)
        {
            heartbeatSeconds = 60;
        }

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_WriteLiveAdvisorHeartbeat, heartbeatSeconds * 1000, true);
    }

    void EoH_WriteLiveAdvisorHeartbeat()
    {
        if (!EoH_LiveAdvisorLogger.m_Config || !EoH_LiveAdvisorLogger.m_Config.Enabled)
        {
            return;
        }

        int playerCount = EoH_GetLiveAdvisorPlayerCount();
        string message = string.Format("Server heartbeat. Players online: %1", playerCount);
        EoH_LiveAdvisorLogger.Log("SERVER_HEARTBEAT", message, "info", "MissionServer");
    }
}
