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
        TryRebuildRuntimeFromPersistence();
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

    void SaveWeeklyEventState()
    {
        EoH_WeeklyEventPersistenceManager.Get().SaveActive(m_ActiveRuntime);
    }

    void SaveWeeklyEventIdleState()
    {
        EoH_WeeklyEventPersistenceManager.Get().SaveIdle();
    }

    bool TryRebuildRuntimeFromPersistence()
    {
        if (m_ActiveRuntime && m_ActiveRuntime.Active)
            return false;

        EoH_WeeklyEventPersistenceState state = EoH_WeeklyEventPersistenceManager.Get().GetState();
        if (!state || !state.HasActiveObjective || state.ObjectiveId == "")
            return false;

        EoH_EventObjective cfg = FindObjectiveById(state.ObjectiveId);
        if (!cfg)
        {
            Print("[EoH_Recovery] runtime_rebuild_failed reason=unknown_objective objective=" + state.ObjectiveId);
            EoH_LiveAdvisorActivity.LogActivity("weekly_event", "recovery_runtime_rebuild_failed reason=unknown_objective objective=" + state.ObjectiveId);
            return false;
        }

        m_ActiveRuntime = new EoH_EventObjectiveRuntime(cfg);
        m_ActiveRuntime.Active = true;
        m_ActiveRuntime.RevealedByIntel = state.RevealedByIntel;
        m_ActiveRuntime.Completed = state.Completed;
        m_ActiveRuntime.RewardUnlocked = state.RewardUnlocked;
        m_ActiveRuntime.StartTime = state.StartTime;
        m_ActiveRuntime.LastTickTime = GetGame().GetTime();
        m_ActiveRuntime.CurrentWave = state.CurrentWave;
        m_ActiveRuntime.RewardCrate = null;

        if (cfg.Id == "altar_relay_towers")
        {
            m_ActiveRuntime.AltarRepairInProgress = false;
            m_ActiveRuntime.AltarRelayOnline = state.RepairCompleted;
            m_ActiveRuntime.AltarRepairProgress01 = 1.0;
            m_ActiveRuntime.AltarRepairStartedAt = state.StartTime;
            m_ActiveRuntime.AltarRewardSpawned = state.RewardUnlocked;
        }

        m_ActiveRuntime.RecoveryRestored = true;
        m_ActiveRuntime.RecoveryPassiveMode = true;

        if (EoH_WeeklyEventConfigManager.Get().IsRecoveryGracePeriodEnabled())
        {
            int now = GetGame().GetTime();
            int graceSeconds = EoH_WeeklyEventConfigManager.Get().GetRecoveryGraceSeconds();
            int graceMs = graceSeconds * 1000;
            m_ActiveRuntime.RecoveryGraceActive = true;
            m_ActiveRuntime.RecoveryGraceStartedAt = now;
            m_ActiveRuntime.RecoveryGraceEndsAt = now + graceMs;
            m_ActiveRuntime.RecoveryGraceExpiredLogged = false;
            Print("[EoH_Recovery] grace_active objective=" + cfg.Id + " seconds=" + graceSeconds.ToString());
            EoH_LiveAdvisorActivity.LogActivity("weekly_event", "recovery_grace_active objective=" + cfg.Id + " seconds=" + graceSeconds.ToString());
        }

        BroadcastObjective();
        Print("[EoH_Recovery] runtime_rebuilt objective=" + cfg.Id + " repaired=" + state.RepairCompleted.ToString() + " wave=" + state.CurrentWave.ToString() + " rewardUnlocked=" + state.RewardUnlocked.ToString() + " passive=true");
        EoH_LiveAdvisorActivity.LogActivity("weekly_event", "recovery_runtime_rebuilt objective=" + cfg.Id + " repaired=" + state.RepairCompleted.ToString() + " wave=" + state.CurrentWave.ToString() + " rewardUnlocked=" + state.RewardUnlocked.ToString() + " passive=true");
        return true;
    }

    bool IsRecoveryGraceActive(int now)
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.RecoveryGraceActive)
            return false;

        if (now < m_ActiveRuntime.RecoveryGraceEndsAt)
            return true;

        m_ActiveRuntime.RecoveryGraceActive = false;
        if (!m_ActiveRuntime.RecoveryGraceExpiredLogged)
        {
            m_ActiveRuntime.RecoveryGraceExpiredLogged = true;
            Print("[EoH_Recovery] grace_expired objective=" + m_ActiveRuntime.Config.Id);
            EoH_LiveAdvisorActivity.LogActivity("weekly_event", "recovery_grace_expired objective=" + m_ActiveRuntime.Config.Id);
        }

        return false;
    }

    void TickRecoveryGrace(int now)
    {
        IsRecoveryGraceActive(now);
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
}
