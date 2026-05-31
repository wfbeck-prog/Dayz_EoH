class EoH_EventObjectiveManager
{
    protected static ref EoH_EventObjectiveManager s_Instance;
    static const bool EOH_FORCE_PURGE_NIGHT_TEST = false;
    static const bool EOH_FORCE_ALTAR_RELAY_TEST = true;
    static const string EOH_PARTICLEPOINT_REWARD_SMOKE = "SmokePoint_3";
    static const float EOH_ALTAR_REPAIR_RADIUS = 5.0;

    protected ref array<ref EoH_EventObjective> m_Objectives;
    protected ref EoH_EventObjectiveRuntime m_ActiveRuntime;
    protected ref array<string> m_UsedWeekendEvents;
    protected ref array<Object> m_PurgeNightAI;
    protected int m_PurgeNightRewardUnlockTime;

    static EoH_EventObjectiveManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_EventObjectiveManager();

        return s_Instance;
    }

    void EoH_EventObjectiveManager()
    {
        m_Objectives = new array<ref EoH_EventObjective>();
        m_UsedWeekendEvents = new array<string>();
        m_PurgeNightAI = new array<Object>();
        m_PurgeNightRewardUnlockTime = 0;
        RegisterDefaults();
        Print("[EoH_EventObjectives] Manager initialized objectives=" + m_Objectives.Count().ToString());
    }

    EoH_EventObjective GetActiveObjectiveConfig()
    {
        if (!m_ActiveRuntime)
            return null;
        return m_ActiveRuntime.Config;
    }

    bool IsIntelAvailable()
    {
        if (!m_ActiveRuntime)
            return true;
        if (!m_ActiveRuntime.Active)
            return true;
        return false;
    }

    bool HasActiveObjective()
    {
        return m_ActiveRuntime && m_ActiveRuntime.Active;
    }

    bool IsObjectiveCombatStarted()
    {
        return m_ActiveRuntime && m_ActiveRuntime.Active && m_ActiveRuntime.StartTime > 0;
    }

    bool WasUsedThisWeekend(string eventId)
    {
        if (!m_UsedWeekendEvents || eventId == "")
            return false;
        return m_UsedWeekendEvents.Find(eventId) >= 0;
    }

    void MarkUsedThisWeekend(string eventId)
    {
        if (!m_UsedWeekendEvents || eventId == "")
            return;
        if (m_UsedWeekendEvents.Find(eventId) < 0)
        {
            m_UsedWeekendEvents.Insert(eventId);
            Print("[EoH_EventObjectives] Marked weekend event used id=" + eventId);
        }
    }

    int GetPurgePhaseOneMs(EoH_EventObjective cfg)
    {
        if (EOH_FORCE_PURGE_NIGHT_TEST)
            return 1 * 60 * 1000;
        return 10 * 60 * 1000;
    }

    int GetPurgePhaseTwoMs(EoH_EventObjective cfg)
    {
        if (EOH_FORCE_PURGE_NIGHT_TEST)
            return 3 * 60 * 1000;
        return 20 * 60 * 1000;
    }

    int GetPurgePhaseThreeMs(EoH_EventObjective cfg)
    {
        if (EOH_FORCE_PURGE_NIGHT_TEST)
            return 4 * 60 * 1000;
        return 25 * 60 * 1000;
    }

    int GetPurgeRewardCleanupMs()
    {
        return 10 * 60 * 1000;
    }

    void RegisterDefaults()
    {
        EoH_EventObjective altarTowers = new EoH_EventObjective();
        altarTowers.Id = "altar_relay_towers";
        altarTowers.DisplayName = "Altar Relay Towers";
        altarTowers.Position = "8130.686035 492.125732 9085.325195";
        altarTowers.ObjectiveType = "relay_tower";
        altarTowers.SpawnObject = "";
        altarTowers.RecommendedPlayers = 8;
        altarTowers.LootTier = 4;
        altarTowers.EnableAIBandits = false;
        altarTowers.EnableExpansionAI = true;
        altarTowers.EnableInfected = true;
        altarTowers.EnableMarker = true;
        altarTowers.EnableSmoke = false;
        altarTowers.DurationMinutes = 60;
        altarTowers.Radius = 250.0;
        m_Objectives.Insert(altarTowers);

        EoH_EventObjective purgeNight = new EoH_EventObjective();
        purgeNight.Id = "purge_night_novy_stary";
        purgeNight.DisplayName = "Purge Night: Central Corridor";
        purgeNight.Position = "6600 300 7700";
        purgeNight.ObjectiveType = "purge_night";
        purgeNight.SpawnObject = "";
        purgeNight.RecommendedPlayers = 12;
        purgeNight.LootTier = 4;
        purgeNight.EnableAIBandits = false;
        purgeNight.EnableExpansionAI = true;
        purgeNight.EnableInfected = true;
        purgeNight.EnableMarker = true;
        purgeNight.EnableSmoke = true;
        purgeNight.DurationMinutes = 30;
        if (EOH_FORCE_PURGE_NIGHT_TEST)
            purgeNight.DurationMinutes = 5;
        purgeNight.Radius = 500.0;
        m_Objectives.Insert(purgeNight);
    }

    EoH_EventObjective FindObjectiveById(string eventId)
    {
        if (!m_Objectives || eventId == "")
            return null;
        foreach (EoH_EventObjective obj : m_Objectives)
        {
            if (obj && obj.Id == eventId)
                return obj;
        }
        return null;
    }

    EoH_EventObjective PickRandomObjective()
    {
        if (!m_Objectives || m_Objectives.Count() == 0)
            return null;

        if (EOH_FORCE_ALTAR_RELAY_TEST)
        {
            EoH_EventObjective altar = FindObjectiveById("altar_relay_towers");
            if (altar && !WasUsedThisWeekend(altar.Id))
            {
                Print("[EoH_EventObjectives] Force test selected Altar Relay");
                return altar;
            }
        }

        if (EOH_FORCE_PURGE_NIGHT_TEST)
        {
            EoH_EventObjective purge = FindObjectiveById("purge_night_novy_stary");
            if (purge && !WasUsedThisWeekend(purge.Id))
            {
                Print("[EoH_EventObjectives] Force test selected Purge Night");
                return purge;
            }
        }
        ref array<ref EoH_EventObjective> available = new array<ref EoH_EventObjective>();
        foreach (EoH_EventObjective candidate : m_Objectives)
        {
            if (candidate && !WasUsedThisWeekend(candidate.Id))
                available.Insert(candidate);
        }
        if (available.Count() == 0)
        {
            Print("[EoH_EventObjectives] No unused weekend events remain for this server session");
            return null;
        }
        return available.Get(Math.RandomInt(0, available.Count()));
    }

    bool StartRandomObjective()
    {
        return RevealRandomObjectiveOnly();
    }

    bool RevealRandomObjectiveOnly()
    {
        if (!IsIntelAvailable())
        {
            Print("[EoH_EventObjectives] Intel reveal blocked active objective already exists");
            return false;
        }
        EoH_EventObjective obj = PickRandomObjective();
        if (!obj)
            return false;
        MarkUsedThisWeekend(obj.Id);
        m_ActiveRuntime = new EoH_EventObjectiveRuntime(obj);
        m_ActiveRuntime.Active = true;
        m_ActiveRuntime.StartTime = 0;
        m_ActiveRuntime.LastTickTime = GetGame().GetTime();
        m_ActiveRuntime.RevealedByIntel = true;
        m_ActiveRuntime.RewardCrate = null;
        m_ActiveRuntime.CurrentWave = 0;
        m_PurgeNightRewardUnlockTime = 0;
        CleanupPurgeNightAI();
        if (obj.ObjectiveType == "purge_night")
            StartPurgeNightRuntime();
        else
        {
            SpawnObjectiveObject();
            BroadcastObjective();
            if (obj.Id == "altar_relay_towers")
                StartAltarRepairWatcher();
        }
        Print("[EoH_EventObjectives] Revealed objective id=" + obj.Id + " type=" + obj.ObjectiveType + " pos=" + obj.Position.ToString());
        return true;
    }

    void StartAltarRepairWatcher()
    {
        int intervalMs = EoH_WeeklyEventConfigManager.Get().GetAltarRepairWatcherIntervalMs();
        float radius = EoH_WeeklyEventConfigManager.Get().GetAltarRepairProximityRadius();
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(TickAltarRelayRepairProximity);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(TickAltarRelayRepairProximity, intervalMs, true);
        Print("[EoH_AltarRepair] Started dedicated proximity watcher intervalMs=" + intervalMs.ToString() + " radius=" + radius.ToString() + " autoRepair=" + EoH_WeeklyEventConfigManager.Get().IsAltarProximityAutoRepairEnabled().ToString());
    }

    void StopAltarRepairWatcher()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(TickAltarRelayRepairProximity);
        Print("[EoH_AltarRepair] Stopped dedicated proximity watcher");
    }

    void StartPurgeNightRuntime()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;
        EoH_EventObjective cfg = m_ActiveRuntime.Config;
        m_ActiveRuntime.StartTime = GetGame().GetTime();
        m_ActiveRuntime.LastTickTime = m_ActiveRuntime.StartTime;
        m_ActiveRuntime.RewardCrate = null;
        m_ActiveRuntime.CurrentWave = 0;
        m_PurgeNightRewardUnlockTime = 0;
        string msg = "Red Ledger purge traffic intercepted. The marked corridor is blacked out for " + cfg.DurationMinutes.ToString() + " minutes. Survivors entering the zone are on their own.";
        EoH_Notifications.SendToAll("PURGE NIGHT", msg);
        BroadcastObjective();
        Print("[EoH_PurgeNight] Started id=" + cfg.Id + " durationMinutes=" + cfg.DurationMinutes.ToString() + " radius=" + cfg.Radius.ToString() + " crateStaged=false");
    }

    bool ActivateObjectiveFromRepair(PlayerBase player)
    {
        return StartAltarRepairAttempt(player);
    }

    bool StartAltarRepairAttempt(PlayerBase player)
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Active || !m_ActiveRuntime.Config)
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "No active relay signal is available for repair.");
            return false;
        }

        EoH_EventObjective cfg = m_ActiveRuntime.Config;
        if (cfg.Id != "altar_relay_towers")
            return false;

        if (m_ActiveRuntime.AltarRelayOnline)
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "This relay has already been restored.");
            return false;
        }

        if (m_ActiveRuntime.AltarRepairInProgress)
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "A repair team is already maintaining the signal tether.");
            return false;
        }

        m_ActiveRuntime.StartTime = GetGame().GetTime();
        m_ActiveRuntime.LastTickTime = m_ActiveRuntime.StartTime;
        m_ActiveRuntime.AltarRepairStartedAt = m_ActiveRuntime.StartTime;
        m_ActiveRuntime.AltarRepairLastProgressLog = 0;
        m_ActiveRuntime.AltarRepairProgress01 = 0.0;
        m_ActiveRuntime.AltarRepairInProgress = true;
        m_ActiveRuntime.AltarRelayOnline = false;
        m_ActiveRuntime.AltarRewardSpawned = false;
        m_ActiveRuntime.AltarRepairGroupId = GetAltarRepairGroupId(player);
        m_ActiveRuntime.AltarRepairStartedById = "";
        m_ActiveRuntime.AltarRepairStartedByName = "unknown";
        if (player && player.GetIdentity())
        {
            m_ActiveRuntime.AltarRepairStartedById = player.GetIdentity().GetId();
            m_ActiveRuntime.AltarRepairStartedByName = player.GetIdentity().GetName();
        }

        EoH_MarkerService.RemoveFromAll("EOH_EVENT_INTEL_ALTAR_RELAY");

        if (EoH_WeeklyEventConfigManager.Get().ShouldStartAltarAIOnRepairStart())
        {
            EoH_EventWaveManager.Get().SpawnWave(m_ActiveRuntime, 1);
            m_ActiveRuntime.CurrentWave = 1;
        }

        EoH_Notifications.SendToAll("ALTAR RELAY BREACH", cfg.DisplayName + " repair has started. Hostile response is beginning. Survive all 5 waves to secure the cache.");
        EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "Signal tether active. Maintain control within 75m until repair completes. Hostile response continues for 5 waves.");
        SendAltarRepairProgressToGroup("RPC_Show", "Hold 75m until repair completes. 5-wave hostile response active.", false, false);
        Print("[EoH_AltarRepair] Started repair attempt group=" + m_ActiveRuntime.AltarRepairGroupId + " by=" + m_ActiveRuntime.AltarRepairStartedByName);
        return true;
    }

    bool IsPlayerNearActiveObjective(PlayerBase player, float radius)
    {
        if (!player || !m_ActiveRuntime || !m_ActiveRuntime.Active || !m_ActiveRuntime.Config)
            return false;
        return vector.Distance(player.GetPosition(), m_ActiveRuntime.Config.Position) <= radius;
    }

    void Tick()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Active || !m_ActiveRuntime.Config)
            return;

        if (m_ActiveRuntime.StartTime <= 0)
        {
            TickAltarRelayRepairProximity();
            return;
        }

        int now = GetGame().GetTime();

        if (m_ActiveRuntime.Config.Id == "altar_relay_towers" && m_ActiveRuntime.AltarRepairInProgress)
            TickAltarRepairRuntime(now);

        if (m_ActiveRuntime.LastTickTime > 0 && now - m_ActiveRuntime.LastTickTime < 30000)
            return;
        m_ActiveRuntime.LastTickTime = now;
        if (m_ActiveRuntime.Config.ObjectiveType == "purge_night")
            TickPurgeNight(now);
        else
        {
            TickWaves(now);
            TickRewardCrate();
        }
    }

    void TickAltarRelayRepairProximity()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Active || !m_ActiveRuntime.Config)
            return;

        EoH_EventObjective cfg = m_ActiveRuntime.Config;
        if (cfg.Id != "altar_relay_towers")
            return;

        if (m_ActiveRuntime.StartTime > 0)
        {
            StopAltarRepairWatcher();
            return;
        }

        float repairRadius = EoH_WeeklyEventConfigManager.Get().GetAltarRepairProximityRadius();
        bool autoRepair = EoH_WeeklyEventConfigManager.Get().IsAltarProximityAutoRepairEnabled();

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        Print("[EoH_AltarRepair][WATCH] players=" + players.Count().ToString() + " radius=" + repairRadius.ToString() + " autoRepair=" + autoRepair.ToString() + " pos=" + cfg.Position.ToString());

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player)
                continue;

            string playerName = "unknown";
            if (player.GetIdentity())
                playerName = player.GetIdentity().GetName();

            float dist = vector.Distance(player.GetPosition(), cfg.Position);
            Print("[EoH_AltarRepair][CHECK] player=" + playerName + " dist=" + dist.ToString() + " playerPos=" + player.GetPosition().ToString());

            if (dist > repairRadius)
                continue;

            EntityAI radio = FindAltarRepairRadio(player);
            EntityAI battery = FindItemOnPlayer(player, "CarBattery");

            Print("[EoH_AltarRepair][PROXIMITY] player=" + playerName + " dist=" + dist.ToString() + " hasRadio=" + (radio != null).ToString() + " hasBattery=" + (battery != null).ToString() + " autoRepair=" + autoRepair.ToString());

            if (!radio || !battery)
            {
                EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "The relay terminal is dormant. A Field Transceiver and Car Battery are required.");
                continue;
            }

            if (!autoRepair)
            {
                EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "Repair equipment detected. Use the relay terminal action to restore the uplink.");
                continue;
            }

            ActivateObjectiveFromRepair(player);
            return;
        }
    }

    void TickAltarRepairRuntime(int now)
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config || !m_ActiveRuntime.AltarRepairInProgress)
            return;

        int members = CountAliveRepairGroupMembersInZone();
        if (members <= 0 && EoH_WeeklyEventConfigManager.Get().ShouldResetAltarRepairWhenGroupLeavesZone())
        {
            FailAltarRepairAttempt("Repair team eliminated or pushed out of the 75m signal zone.");
            return;
        }

        float durationMs = EoH_WeeklyEventConfigManager.Get().GetAltarRepairDurationSeconds() * 1000.0;
        if (durationMs <= 0)
            durationMs = 60000.0;

        float elapsed = now - m_ActiveRuntime.AltarRepairStartedAt;
        m_ActiveRuntime.AltarRepairProgress01 = Math.Clamp(elapsed / durationMs, 0.0, 1.0);

        int progressPct = Math.Round(m_ActiveRuntime.AltarRepairProgress01 * 100.0);
        if (progressPct >= m_ActiveRuntime.AltarRepairLastProgressLog + 10)
        {
            m_ActiveRuntime.AltarRepairLastProgressLog = progressPct;
            Print("[EoH_AltarRepair][PROGRESS] pct=" + progressPct.ToString() + " membersInZone=" + members.ToString() + " group=" + m_ActiveRuntime.AltarRepairGroupId);
        }

        SendAltarRepairProgressToGroup("RPC_Update", "Repairing relay. Hostile response continues for 5 waves.", false, false);

        if (m_ActiveRuntime.AltarRepairProgress01 >= 1.0)
            CompleteAltarRepair();
    }

    void FailAltarRepairAttempt(string reason)
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;

        EoH_Notifications.SendToAll("REPAIR FAILED", reason + " Relay remains offline.");
        SendAltarRepairProgressToGroup("RPC_Update", "REPAIR FAILED - " + reason, false, true);
        Print("[EoH_AltarRepair][FAILED] " + reason + " previousGroup=" + m_ActiveRuntime.AltarRepairGroupId);

        m_ActiveRuntime.AltarRepairInProgress = false;
        m_ActiveRuntime.AltarRepairProgress01 = 0.0;
        m_ActiveRuntime.AltarRepairStartedAt = 0;
        m_ActiveRuntime.AltarRepairLastProgressLog = 0;
        m_ActiveRuntime.AltarRepairGroupId = "";
        m_ActiveRuntime.AltarRepairStartedById = "";
        m_ActiveRuntime.AltarRepairStartedByName = "";
        m_ActiveRuntime.StartTime = 0;
        m_ActiveRuntime.LastTickTime = GetGame().GetTime();
        m_ActiveRuntime.CurrentWave = 0;
        m_ActiveRuntime.AltarRewardSpawned = false;
        m_ActiveRuntime.RewardCrate = null;

        StartAltarRepairWatcher();
    }

    void CompleteAltarRepair()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;

        PlayerBase payer = FindRepairGroupMemberWithComponents();
        if (EoH_WeeklyEventConfigManager.Get().ShouldConsumeAltarRepairItemsAtCompletion())
        {
            if (!payer)
            {
                FailAltarRepairAttempt("Repair components missing from the active repair team.");
                return;
            }

            EntityAI radio = FindAltarRepairRadio(payer);
            EntityAI battery = FindItemOnPlayer(payer, "CarBattery");
            if (!radio || !battery)
            {
                FailAltarRepairAttempt("Repair components missing from the active repair team.");
                return;
            }

            GetGame().ObjectDelete(radio);
            GetGame().ObjectDelete(battery);
        }

        m_ActiveRuntime.AltarRepairInProgress = false;
        m_ActiveRuntime.AltarRelayOnline = true;
        m_ActiveRuntime.AltarRepairProgress01 = 1.0;

        EoH_Notifications.SendToAll("RELAY ONLINE", m_ActiveRuntime.Config.DisplayName + " communications restored. The fight is not over. Survive all 5 waves to secure the cache.");
        SendAltarRepairProgressToGroup("RPC_Update", "RELAY ONLINE - Survive all 5 waves to secure the cache.", true, false);
        Print("[EoH_AltarRepair] Relay online group=" + m_ActiveRuntime.AltarRepairGroupId);
    }

    EoH_AltarRepairProgressData BuildAltarRepairProgressData(string status, bool complete, bool failed)
    {
        EoH_AltarRepairProgressData data = new EoH_AltarRepairProgressData();

        if (!m_ActiveRuntime)
            return data;

        data.Progress01 = m_ActiveRuntime.AltarRepairProgress01;
        data.ProgressPercent = Math.Round(data.Progress01 * 100.0);
        data.MembersInZone = CountAliveRepairGroupMembersInZone();
        data.MaintainRadius = EoH_WeeklyEventConfigManager.Get().GetAltarRepairMaintainRadius();
        data.Title = "ALTAR RELAY REPAIR";
        data.Status = status;
        data.Complete = complete;
        data.Failed = failed;

        if (data.ProgressPercent < 0)
            data.ProgressPercent = 0;
        if (data.ProgressPercent > 100)
            data.ProgressPercent = 100;

        return data;
    }

    void SendAltarRepairProgressToGroup(string rpcName, string status, bool complete, bool failed)
    {
        if (!m_ActiveRuntime || m_ActiveRuntime.AltarRepairGroupId == "")
            return;

        EoH_AltarRepairProgressData data = BuildAltarRepairProgressData(status, complete, failed);
        Param1<EoH_AltarRepairProgressData> param = new Param1<EoH_AltarRepairProgressData>(data);

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            if (!IsSameAltarRepairGroup(player))
                continue;

            GetRPCManager().SendRPC("EoH_AltarRepairProgress", rpcName, param, true, player.GetIdentity());
        }
    }

    string GetAltarRepairGroupId(PlayerBase player)
    {
        string groupId = EoH_GroupHelper.GetGroupID(player);
        if (groupId != "")
            return groupId;

        if (player && player.GetIdentity())
            return "SOLO_" + player.GetIdentity().GetId();

        return "SOLO_unknown";
    }

    bool IsSameAltarRepairGroup(PlayerBase player)
    {
        if (!m_ActiveRuntime || !player)
            return false;

        return GetAltarRepairGroupId(player) == m_ActiveRuntime.AltarRepairGroupId;
    }

    int CountAliveRepairGroupMembersInZone()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return 0;

        int count = 0;
        float radius = EoH_WeeklyEventConfigManager.Get().GetAltarRepairMaintainRadius();
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.IsAlive())
                continue;

        ...