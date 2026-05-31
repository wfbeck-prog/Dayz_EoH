class EoH_EventObjectiveManager
{
    protected static ref EoH_EventObjectiveManager s_Instance;
    static const bool EOH_FORCE_PURGE_NIGHT_TEST = false;
    static const bool EOH_FORCE_ALTAR_RELAY_TEST = true;
    static const bool EOH_ALLOW_FORCE_TEST_EVENT_REUSE = true;
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
        if (EOH_ALLOW_FORCE_TEST_EVENT_REUSE && (EOH_FORCE_ALTAR_RELAY_TEST || EOH_FORCE_PURGE_NIGHT_TEST))
            return false;

        if (!m_UsedWeekendEvents || eventId == "")
            return false;
        return m_UsedWeekendEvents.Find(eventId) >= 0;
    }

    void MarkUsedThisWeekend(string eventId)
    {
        if (EOH_ALLOW_FORCE_TEST_EVENT_REUSE && (EOH_FORCE_ALTAR_RELAY_TEST || EOH_FORCE_PURGE_NIGHT_TEST))
        {
            Print("[EoH_EventObjectives] Force test reuse enabled; not marking event used id=" + eventId);
            return;
        }

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
            TickAltarRewardCleanup(now);
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

            if (!IsSameAltarRepairGroup(player))
                continue;

            if (vector.Distance(player.GetPosition(), m_ActiveRuntime.Config.Position) <= radius)
                count++;
        }

        return count;
    }

    PlayerBase FindRepairGroupMemberWithComponents()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return null;

        float radius = EoH_WeeklyEventConfigManager.Get().GetAltarRepairMaintainRadius();
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.IsAlive())
                continue;

            if (!IsSameAltarRepairGroup(player))
                continue;

            if (vector.Distance(player.GetPosition(), m_ActiveRuntime.Config.Position) > radius)
                continue;

            if (FindAltarRepairRadio(player) && FindItemOnPlayer(player, "CarBattery"))
                return player;
        }

        return null;
    }

    void SpawnAltarRewardPhase()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;

        if (m_ActiveRuntime.AltarRewardSpawned)
            return;

        if (!m_ActiveRuntime.RewardCrate)
            m_ActiveRuntime.RewardCrate = new EoH_EventRewardCrate();

        SpawnRewardCrate();
        m_ActiveRuntime.AltarRewardSpawned = true;
        m_ActiveRuntime.RewardUnlocked = true;
        m_PurgeNightRewardUnlockTime = GetGame().GetTime();

        bool smokeStarted = SpawnEventSmoke(m_ActiveRuntime.Config.Position, "M18SmokeGrenade_Red");
        EoH_Notifications.SendToAll("ALTAR CACHE DROPPED", "Round 5 has deployed the recovery cache at Altar Relay Towers. Red smoke active=" + smokeStarted.ToString() + ". Recovery window closes in 10 minutes.");
        Print("[EoH_AltarRepair] Reward phase spawned on wave=" + m_ActiveRuntime.CurrentWave.ToString() + " smokeStarted=" + smokeStarted.ToString());
    }

    void TickAltarRewardCleanup(int now)
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;

        if (m_ActiveRuntime.Config.Id != "altar_relay_towers")
            return;

        if (!m_ActiveRuntime.RewardUnlocked || m_PurgeNightRewardUnlockTime <= 0)
            return;

        int cleanupElapsed = now - m_PurgeNightRewardUnlockTime;
        int cleanupMs = GetPurgeRewardCleanupMs();
        Print("[EoH_AltarRepair][CLEANUP] rewardElapsedMs=" + cleanupElapsed.ToString() + " cleanupMs=" + cleanupMs.ToString());

        if (cleanupElapsed >= cleanupMs)
        {
            EoH_Notifications.SendToAll("ALTAR RELAY", "The recovery window has closed. Altar Relay Towers has gone silent.");
            EoH_MarkerService.RemoveFromAll("EOH_EVENT_altar_relay_towers");
            EoH_MarkerService.RemoveFromAll("EOH_EVENT_INTEL_ALTAR_RELAY");
            EndActiveObjective();
        }
    }

    EntityAI FindAltarRepairRadio(PlayerBase player)
    {
        EntityAI radio = FindItemOnPlayer(player, "FieldTransceiver");
        if (radio)
            return radio;

        radio = FindItemOnPlayer(player, "PersonalRadio");
        if (radio)
            return radio;

        radio = FindItemOnPlayer(player, "BaseRadio");
        if (radio)
            return radio;

        radio = FindItemOnPlayer(player, "EoH_FieldRadio");
        if (radio)
            return radio;

        return null;
    }

    EntityAI FindItemOnPlayer(PlayerBase player, string className)
    {
        if (!player || className == "")
            return null;

        array<EntityAI> items = new array<EntityAI>();
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

        foreach (EntityAI item : items)
        {
            if (!item)
                continue;

            if (item.GetType() == className || item.IsKindOf(className))
                return item;
        }

        return null;
    }

    void TickPurgeNight(int now)
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;
        EoH_EventObjective cfg = m_ActiveRuntime.Config;
        int elapsed = now - m_ActiveRuntime.StartTime;
        int durationMs = cfg.DurationMinutes * 60 * 1000;
        int phaseOneMs = GetPurgePhaseOneMs(cfg);
        int phaseTwoMs = GetPurgePhaseTwoMs(cfg);
        int phaseThreeMs = GetPurgePhaseThreeMs(cfg);
        Print("[EoH_PurgeNight][TICK] id=" + cfg.Id + " elapsedMs=" + elapsed.ToString() + " durationMs=" + durationMs.ToString() + " wave=" + m_ActiveRuntime.CurrentWave.ToString() + " p1=" + phaseOneMs.ToString() + " p2=" + phaseTwoMs.ToString() + " p3=" + phaseThreeMs.ToString());
        if (m_ActiveRuntime.CurrentWave < 1 && elapsed >= phaseOneMs)
        {
            SpawnPurgeNightWave(1);
            EoH_Notifications.SendToAll("PURGE NIGHT", "Raider movement confirmed inside the blackout zone. Hold the corridor or stay clear.");
            m_ActiveRuntime.CurrentWave = 1;
            Print("[EoH_PurgeNight] Phase 1 pressure notification id=" + cfg.Id + " elapsedMs=" + elapsed.ToString());
        }
        if (m_ActiveRuntime.CurrentWave < 2 && elapsed >= phaseTwoMs)
        {
            SpawnPurgeNightWave(2);
            EoH_Notifications.SendToAll("PURGE NIGHT", "The purge signal is intensifying. Teams still inside the corridor are now committed.");
            m_ActiveRuntime.CurrentWave = 2;
            Print("[EoH_PurgeNight] Phase 2 pressure notification id=" + cfg.Id + " elapsedMs=" + elapsed.ToString());
        }
        if (m_ActiveRuntime.CurrentWave < 3 && elapsed >= phaseThreeMs)
        {
            SpawnPurgeNightWave(3);
            EoH_Notifications.SendToAll("PURGE NIGHT", "Final purge window. Recovery cache exposure is imminent.");
            m_ActiveRuntime.CurrentWave = 3;
            Print("[EoH_PurgeNight] Phase 3 final notification id=" + cfg.Id + " elapsedMs=" + elapsed.ToString());
        }
        if (elapsed >= durationMs)
            UnlockPurgeNightReward();
        if (m_ActiveRuntime && m_ActiveRuntime.RewardUnlocked && m_PurgeNightRewardUnlockTime > 0)
        {
            int cleanupElapsed = now - m_PurgeNightRewardUnlockTime;
            int cleanupMs = GetPurgeRewardCleanupMs();
            Print("[EoH_PurgeNight][CLEANUP] rewardElapsedMs=" + cleanupElapsed.ToString() + " cleanupMs=" + cleanupMs.ToString());
            if (cleanupElapsed >= cleanupMs)
            {
                EoH_Notifications.SendToAll("PURGE NIGHT", "The recovery window has closed. Remaining cache markers are being cleared.");
                EndActiveObjective();
            }
        }
    }

    void SpawnPurgeNightWave(int wave)
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;
        EoH_EventObjective cfg = m_ActiveRuntime.Config;
        int count = 3;
        string loadout = "EoH_AI_Patrol_Assault_DF";
        if (wave == 2)
        {
            count = 4;
            loadout = "EoH_AI_Patrol_Marksman_DF";
        }
        else if (wave >= 3)
        {
            count = 5;
            loadout = "EoH_AI_HighValue_Hard";
        }
        eAIGroup group = EoH_TownAISpawnAdapter.CreateTownPatrolGroup(cfg.Position);
        for (int i = 0; i < count; i++)
        {
            vector spawnPos = GetPurgeNightSpawnPosition(cfg.Position, 80.0, Math.Min(260.0, cfg.Radius));
            Object obj = EoH_TownAISpawnAdapter.SpawnTownPatrolUnit("Purge Night", spawnPos, cfg.Position, loadout, group);
            if (obj)
                m_PurgeNightAI.Insert(obj);
        }
        Print("[EoH_PurgeNight][AI] Spawned wave=" + wave.ToString() + " count=" + count.ToString() + " loadout=" + loadout + " trackedAI=" + m_PurgeNightAI.Count().ToString());
    }

    vector GetPurgeNightSpawnPosition(vector center, float minRadius, float maxRadius)
    {
        float angle = Math.RandomFloat(0, 6.28318);
        float dist = Math.RandomFloat(minRadius, maxRadius);
        vector pos = center;
        pos[0] = center[0] + Math.Cos(angle) * dist;
        pos[2] = center[2] + Math.Sin(angle) * dist;
        pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.1;
        return pos;
    }

    void CleanupPurgeNightAI()
    {
        if (!m_PurgeNightAI)
            m_PurgeNightAI = new array<Object>();
        for (int i = m_PurgeNightAI.Count() - 1; i >= 0; i--)
        {
            Object obj = m_PurgeNightAI.Get(i);
            if (obj)
                GetGame().ObjectDelete(obj);
            m_PurgeNightAI.Remove(i);
        }
        Print("[EoH_PurgeNight][AI] Cleanup complete");
    }

    void UnlockPurgeNightReward()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;
        if (m_ActiveRuntime.RewardUnlocked)
            return;
        if (!m_ActiveRuntime.RewardCrate)
            m_ActiveRuntime.RewardCrate = new EoH_EventRewardCrate();
        SpawnRewardCrate();
        if (!m_ActiveRuntime.RewardCrate)
            return;
        m_ActiveRuntime.RewardCrate.MarkUnlocked();
        m_ActiveRuntime.RewardUnlocked = true;
        m_PurgeNightRewardUnlockTime = GetGame().GetTime();
        bool smokeStarted = SpawnEventSmoke(m_ActiveRuntime.RewardCrate.Position, "M18SmokeGrenade_Green");
        string msg = "The purge signal has burned out. A recovery cache is now exposed inside the marked corridor.";
        if (!smokeStarted)
            msg += " No smoke confirmation received; use the event marker and field report coordinates.";
        EoH_Notifications.SendToAll("PURGE COMPLETE", msg);
        EoH_Notifications.SendToAll("RECOVERY CACHE", "The Purge Night cache has spawned and is vulnerable for 10 minutes. Move to the marked corridor and secure the payout.");
        Print("[EoH_PurgeNight] Reward spawned id=" + m_ActiveRuntime.Config.Id + " smokeStarted=" + smokeStarted.ToString() + " cleanupMs=" + GetPurgeRewardCleanupMs().ToString() + " cratePos=" + m_ActiveRuntime.RewardCrate.Position.ToString());
    }

    void TickWaves(int now)
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Active)
            return;

        int elapsed = now - m_ActiveRuntime.StartTime;

        if (m_ActiveRuntime.Config && m_ActiveRuntime.Config.Id == "altar_relay_towers")
        {
            int totalRounds = EoH_WeeklyEventConfigManager.Get().GetAltarAIRounds();
            int rewardWave = EoH_WeeklyEventConfigManager.Get().GetAltarRewardWave();

            if (m_ActiveRuntime.CurrentWave < 2 && totalRounds >= 2 && elapsed >= EoH_WeeklyEventConfigManager.Get().GetAltarWaveDelaySeconds(2) * 1000)
            {
                EoH_EventWaveManager.Get().SpawnWave(m_ActiveRuntime, 2);
                m_ActiveRuntime.CurrentWave = 2;
                if (rewardWave == 2)
                    SpawnAltarRewardPhase();
                return;
            }

            if (m_ActiveRuntime.CurrentWave < 3 && totalRounds >= 3 && elapsed >= EoH_WeeklyEventConfigManager.Get().GetAltarWaveDelaySeconds(3) * 1000)
            {
                EoH_EventWaveManager.Get().SpawnWave(m_ActiveRuntime, 3);
                m_ActiveRuntime.CurrentWave = 3;
                if (rewardWave == 3)
                    SpawnAltarRewardPhase();
                return;
            }

            if (m_ActiveRuntime.CurrentWave < 4 && totalRounds >= 4 && elapsed >= EoH_WeeklyEventConfigManager.Get().GetAltarWaveDelaySeconds(4) * 1000)
            {
                EoH_EventWaveManager.Get().SpawnWave(m_ActiveRuntime, 4);
                m_ActiveRuntime.CurrentWave = 4;
                if (rewardWave == 4)
                    SpawnAltarRewardPhase();
                return;
            }

            if (m_ActiveRuntime.CurrentWave < 5 && totalRounds >= 5 && elapsed >= EoH_WeeklyEventConfigManager.Get().GetAltarWaveDelaySeconds(5) * 1000)
            {
                EoH_EventWaveManager.Get().SpawnWave(m_ActiveRuntime, 5);
                m_ActiveRuntime.CurrentWave = 5;
                if (rewardWave == 5)
                    SpawnAltarRewardPhase();
                return;
            }

            return;
        }

        if (m_ActiveRuntime.CurrentWave < 2 && elapsed >= 10 * 60 * 1000)
        {
            EoH_EventWaveManager.Get().SpawnWave(m_ActiveRuntime, 2);
            m_ActiveRuntime.CurrentWave = 2;
            return;
        }
        if (m_ActiveRuntime.CurrentWave < 3 && elapsed >= 20 * 60 * 1000)
        {
            EoH_EventWaveManager.Get().SpawnWave(m_ActiveRuntime, 3);
            m_ActiveRuntime.CurrentWave = 3;
            return;
        }
    }

    void SpawnObjectiveObject()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;
        EoH_EventObjective cfg = m_ActiveRuntime.Config;
        if (cfg.SpawnObject == "")
            return;
        Object obj = GetGame().CreateObject(cfg.SpawnObject, cfg.Position);
        if (!obj)
        {
            Print("[EoH_EventObjectives][WARN] Failed spawning objective object type=" + cfg.SpawnObject);
            return;
        }
        m_ActiveRuntime.SpawnedObject = obj;
    }

    void SpawnRewardCrate()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config || !m_ActiveRuntime.RewardCrate)
            return;
        EoH_EventObjective cfg = m_ActiveRuntime.Config;
        vector cratePos = cfg.Position + "8 0 8";
        cratePos[1] = GetGame().SurfaceY(cratePos[0], cratePos[2]);
        CleanupRewardObjects(cratePos, 15.0);
        m_ActiveRuntime.RewardCrate.SetRuntime(cratePos, 25, cfg.LootTier);
        Object crate = GetGame().CreateObject(m_ActiveRuntime.RewardCrate.CrateType, cratePos);
        m_ActiveRuntime.RewardCrate.MarkSpawned(crate);
        EntityAI crateAI = EntityAI.Cast(crate);
        if (crateAI)
            EoH_WeeklyEventRewardConfigManager.Get().FillRewardCrate(crateAI, cfg.Id);
        Print("[EoH_EventObjectives] Reward crate spawned id=" + cfg.Id + " pos=" + cratePos.ToString() + " surfaceY=" + cratePos[1].ToString());
    }

    void AddRewardItem(EntityAI container, string className, int count)
    {
        if (!container || className == "" || count <= 0)
            return;
        for (int i = 0; i < count; i++)
        {
            EntityAI item = container.GetInventory().CreateInInventory(className);
            if (!item)
                Print("[EoH_PurgeNight][Reward][WARN] Failed to add item=" + className);
        }
    }

    bool SpawnEventSmoke(vector pos, string smokeType)
    {
        if (smokeType == "")
            smokeType = "M18SmokeGrenade_Green";
        vector smokePos = pos;
        smokePos[1] = GetGame().SurfaceY(pos[0], pos[2]);
        CleanupRewardSmoke(smokePos, 15.0);
        SmokeGrenadeBase smoke = SmokeGrenadeBase.Cast(GetGame().CreateObjectEx(smokeType, smokePos, ECE_PLACE_ON_SURFACE));
        if (smoke)
        {
            if (smoke.GetCompEM() && smoke.GetCompEM().CanWork())
                smoke.GetCompEM().SwitchOn();
            smoke.SetLifetime(1800);
            Print("[EoH_EventObjectives] Spawned event grenade smoke type=" + smokeType + " pos=" + smokePos.ToString());
            return true;
        }
        Print("[EoH_EventObjectives][WARN] Grenade smoke failed type=" + smokeType + " pos=" + smokePos.ToString() + " trying ParticlePoints=" + EOH_PARTICLEPOINT_REWARD_SMOKE);
        return SpawnParticlePointSmoke(smokePos);
    }

    bool SpawnParticlePointSmoke(vector smokePos)
    {
        smokePos[1] = GetGame().SurfaceY(smokePos[0], smokePos[2]);
        CleanupRewardSmoke(smokePos, 15.0);
        Object smokePoint = GetGame().CreateObjectEx(EOH_PARTICLEPOINT_REWARD_SMOKE, smokePos, ECE_PLACE_ON_SURFACE);
        if (!smokePoint)
        {
            Print("[EoH_EventObjectives][WARN] ParticlePoints smoke failed class=" + EOH_PARTICLEPOINT_REWARD_SMOKE + " pos=" + smokePos.ToString());
            return false;
        }
        Print("[EoH_EventObjectives] Spawned ParticlePoints smoke class=" + EOH_PARTICLEPOINT_REWARD_SMOKE + " pos=" + smokePos.ToString());
        return true;
    }

    void CleanupRewardObjects(vector pos, float radius)
    {
        array<Object> objects = new array<Object>();
        array<CargoBase> cargos = new array<CargoBase>();
        GetGame().GetObjectsAtPosition(pos, radius, objects, cargos);
        foreach (Object obj : objects)
        {
            if (!obj)
                continue;
            string type = obj.GetType();
            if (type == "SeaChest" || type == "WoodenCrate" || type == "AmmoBox" || type.Contains("M18SmokeGrenade") || type == EOH_PARTICLEPOINT_REWARD_SMOKE || type == "EoH_RewardSmokeMarker")
            {
                Print("[EoH_EventObjectives] Cleanup reward object type=" + type + " pos=" + obj.GetPosition().ToString());
                GetGame().ObjectDelete(obj);
            }
        }
    }

    void CleanupRewardSmoke(vector pos, float radius)
    {
        array<Object> objects = new array<Object>();
        array<CargoBase> cargos = new array<CargoBase>();
        GetGame().GetObjectsAtPosition(pos, radius, objects, cargos);
        foreach (Object obj : objects)
        {
            if (!obj)
                continue;
            string type = obj.GetType();
            if (type.Contains("M18SmokeGrenade") || type == EOH_PARTICLEPOINT_REWARD_SMOKE || type == "EoH_RewardSmokeMarker")
            {
                Print("[EoH_EventObjectives] Cleanup reward smoke type=" + type + " pos=" + obj.GetPosition().ToString());
                GetGame().ObjectDelete(obj);
            }
        }
    }

    void TickRewardCrate()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.RewardCrate)
            return;
        if (!m_ActiveRuntime.RewardCrate.ShouldUnlock())
            return;
        m_ActiveRuntime.RewardCrate.MarkUnlocked();
        m_ActiveRuntime.RewardUnlocked = true;
        bool smokeStarted = SpawnEventSmoke(m_ActiveRuntime.RewardCrate.Position, "M18SmokeGrenade_Green");
        EoH_Notifications.SendToAll("EVENT EXTRACTION", "The secured cache at " + m_ActiveRuntime.Config.DisplayName + " is now vulnerable. Smoke active=" + smokeStarted.ToString());
        Print("[EoH_EventObjectives] Reward crate unlocked id=" + m_ActiveRuntime.Config.Id + " smokeStarted=" + smokeStarted.ToString());
    }

    void BroadcastObjective()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;
        EoH_EventObjective cfg = m_ActiveRuntime.Config;
        string eventMsg;
        if (cfg.ObjectiveType == "purge_night")
            eventMsg = "Red Ledger purge broadcast active near " + cfg.DisplayName + ". Fight for the corridor until the signal burns out.";
        else
            eventMsg = "High-value signal activity detected near " + cfg.DisplayName + ". Field repair required.";
        EoH_Notifications.SendToAll("WEEKEND EVENT", eventMsg);
        if (!cfg.EnableMarker)
            return;
        EoH_MarkerData data = new EoH_MarkerData("EOH_EVENT_" + cfg.Id, cfg.DisplayName, cfg.Position);
        data.Category = "event";
        data.Icon = "Radio";
        data.Is3D = 1;
        data.Pulse = 1;
        data.Color = ARGB(255, 255, 80, 0);
        data.BaseColor = data.Color;
        data.Normalize();
        EoH_MarkerService.Broadcast(data);
    }

    void CompleteActiveObjective()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;
        m_ActiveRuntime.Completed = true;
        EndActiveObjective();
    }

    void EndActiveObjective()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;
        StopAltarRepairWatcher();
        EoH_EventObjective cfg = m_ActiveRuntime.Config;
        if (m_ActiveRuntime.SpawnedObject)
            GetGame().ObjectDelete(m_ActiveRuntime.SpawnedObject);
        if (m_ActiveRuntime.RewardCrate)
            m_ActiveRuntime.RewardCrate.Cleanup();
        CleanupRewardObjects(cfg.Position, cfg.Radius);
        CleanupPurgeNightAI();
        m_PurgeNightRewardUnlockTime = 0;
        EoH_MarkerService.RemoveFromAll("EOH_EVENT_" + cfg.Id);
        EoH_MarkerService.RemoveFromAll("EOH_EVENT_INTEL_ALTAR_RELAY");
        EoH_Notifications.SendToAll("WEEKEND EVENT", cfg.DisplayName + " has gone silent. Intel channels are open again.");
        Print("[EoH_EventObjectives] Ended objective id=" + cfg.Id + " intelAvailable=true cleanupRadius=" + cfg.Radius.ToString());
        m_ActiveRuntime = null;
    }
}