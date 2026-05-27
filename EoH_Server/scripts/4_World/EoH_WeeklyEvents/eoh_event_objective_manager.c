class EoH_EventObjectiveManager
{
    protected static ref EoH_EventObjectiveManager s_Instance;
    static const bool EOH_FORCE_PURGE_NIGHT_TEST = true;

    protected ref array<ref EoH_EventObjective> m_Objectives;
    protected ref EoH_EventObjectiveRuntime m_ActiveRuntime;
    protected ref array<string> m_UsedWeekendEvents;

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

    void RegisterDefaults()
    {
        EoH_EventObjective altarTowers = new EoH_EventObjective();
        altarTowers.Id = "altar_relay_towers";
        altarTowers.DisplayName = "Altar Relay Towers";
        altarTowers.Position = "8132.95068359375 492.1257629394531 9093.74609375";
        altarTowers.ObjectiveType = "relay_tower";
        altarTowers.SpawnObject = "Land_Tisy_Radar";
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
        purgeNight.DurationMinutes = 60;
        purgeNight.Radius = 500.0;

        m_Objectives.Insert(purgeNight);

        EoH_EventObjective convoy = new EoH_EventObjective();
        convoy.Id = "convoy_blackmountain";
        convoy.DisplayName = "Destroyed Relay Convoy";
        convoy.Position = "11100 0 2550";
        convoy.ObjectiveType = "convoy";
        convoy.SpawnObject = "Land_Wreck_Ural";
        convoy.RecommendedPlayers = 4;
        convoy.LootTier = 3;
        convoy.EnableAIBandits = false;
        convoy.EnableExpansionAI = true;
        convoy.EnableSmoke = false;
        convoy.DurationMinutes = 60;
        convoy.Radius = 250.0;

        m_Objectives.Insert(convoy);
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

        if (obj.ObjectiveType == "purge_night")
            StartPurgeNightRuntime();
        else
        {
            SpawnObjectiveObject();
            BroadcastObjective();
        }

        Print("[EoH_EventObjectives] Revealed objective id=" + obj.Id + " type=" + obj.ObjectiveType + " pos=" + obj.Position.ToString());
        return true;
    }

    void StartPurgeNightRuntime()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;

        EoH_EventObjective cfg = m_ActiveRuntime.Config;
        m_ActiveRuntime.StartTime = GetGame().GetTime();
        m_ActiveRuntime.LastTickTime = m_ActiveRuntime.StartTime;
        m_ActiveRuntime.RewardCrate = new EoH_EventRewardCrate();
        m_ActiveRuntime.CurrentWave = 0;

        SpawnRewardCrate();

        string msg = "Red Ledger purge traffic intercepted. The marked corridor is blacked out for " + cfg.DurationMinutes.ToString() + " minutes. Survivors entering the zone are on their own.";
        EoH_Notifications.SendToAll("PURGE NIGHT", msg);

        BroadcastObjective();

        Print("[EoH_PurgeNight] Started id=" + cfg.Id + " durationMinutes=" + cfg.DurationMinutes.ToString() + " radius=" + cfg.Radius.ToString());
    }

    bool ActivateObjectiveFromRepair(PlayerBase player)
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Active || !m_ActiveRuntime.Config)
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "No active relay signal is available for repair.");
            return false;
        }

        if (m_ActiveRuntime.StartTime > 0)
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "This relay has already been restored.");
            return false;
        }

        EoH_EventObjective cfg = m_ActiveRuntime.Config;
        m_ActiveRuntime.StartTime = GetGame().GetTime();
        m_ActiveRuntime.LastTickTime = m_ActiveRuntime.StartTime;
        m_ActiveRuntime.RewardCrate = new EoH_EventRewardCrate();

        SpawnRewardCrate();
        SpawnEventSmoke(cfg.Position, "M18SmokeGrenade_Red");
        EoH_EventWaveManager.Get().SpawnWave(m_ActiveRuntime, 1);
        m_ActiveRuntime.CurrentWave = 1;

        EoH_MarkerService.RemoveFromAll("EOH_EVENT_INTEL_ALTAR_RELAY");

        string relayMsg = cfg.DisplayName + " has been restored. Hostile contact is moving toward the signal.";
        EoH_Notifications.SendToAll("RELAY ONLINE", relayMsg);

        if (player && player.GetIdentity())
            Print("[EoH_EventObjectives] Relay repaired id=" + cfg.Id + " player=" + player.GetIdentity().GetName());
        else
            Print("[EoH_EventObjectives] Relay repaired id=" + cfg.Id);

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
            return;

        int now = GetGame().GetTime();
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

    void TickPurgeNight(int now)
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;

        EoH_EventObjective cfg = m_ActiveRuntime.Config;
        int elapsed = now - m_ActiveRuntime.StartTime;
        int durationMs = cfg.DurationMinutes * 60 * 1000;

        if (m_ActiveRuntime.CurrentWave < 1 && elapsed >= 10 * 60 * 1000)
        {
            EoH_Notifications.SendToAll("PURGE NIGHT", "Raider movement confirmed inside the blackout zone. Expansion AI pressure should be active where configured.");
            m_ActiveRuntime.CurrentWave = 1;
            Print("[EoH_PurgeNight] Phase 1 pressure notification id=" + cfg.Id);
        }

        if (m_ActiveRuntime.CurrentWave < 2 && elapsed >= 30 * 60 * 1000)
        {
            EoH_Notifications.SendToAll("PURGE NIGHT", "The purge signal is intensifying. Hold the corridor or stay clear.");
            m_ActiveRuntime.CurrentWave = 2;
            Print("[EoH_PurgeNight] Phase 2 pressure notification id=" + cfg.Id);
        }

        if (m_ActiveRuntime.CurrentWave < 3 && elapsed >= 50 * 60 * 1000)
        {
            EoH_Notifications.SendToAll("PURGE NIGHT", "Final purge window. Any cache recovery will happen soon.");
            m_ActiveRuntime.CurrentWave = 3;
            Print("[EoH_PurgeNight] Phase 3 final notification id=" + cfg.Id);
        }

        if (elapsed >= durationMs)
        {
            UnlockPurgeNightReward();
        }
    }

    void UnlockPurgeNightReward()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config || !m_ActiveRuntime.RewardCrate)
            return;

        if (m_ActiveRuntime.RewardUnlocked)
            return;

        m_ActiveRuntime.RewardCrate.MarkUnlocked();
        m_ActiveRuntime.RewardUnlocked = true;

        SpawnEventSmoke(m_ActiveRuntime.RewardCrate.Position, "M18SmokeGrenade_Green");

        string msg = "The purge signal has burned out. A recovery cache is vulnerable inside the marked corridor.";
        EoH_Notifications.SendToAll("PURGE COMPLETE", msg);

        Print("[EoH_PurgeNight] Reward unlocked id=" + m_ActiveRuntime.Config.Id);
    }

    void TickWaves(int now)
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Active)
            return;

        int elapsed = now - m_ActiveRuntime.StartTime;

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

        m_ActiveRuntime.RewardCrate.SetRuntime(cratePos, 25, cfg.LootTier);
        Object crate = GetGame().CreateObject(m_ActiveRuntime.RewardCrate.CrateType, cratePos);
        m_ActiveRuntime.RewardCrate.MarkSpawned(crate);

        Print("[EoH_EventObjectives] Reward crate staged id=" + cfg.Id + " pos=" + cratePos.ToString());
    }

    void SpawnEventSmoke(vector pos, string smokeType)
    {
        if (smokeType == "")
            smokeType = "M18SmokeGrenade_Green";

        vector smokePos = pos;
        smokePos[1] = GetGame().SurfaceY(pos[0], pos[2]);

        SmokeGrenadeBase smoke = SmokeGrenadeBase.Cast(GetGame().CreateObjectEx(smokeType, smokePos, ECE_PLACE_ON_SURFACE));
        if (!smoke)
        {
            Print("[EoH_EventObjectives][WARN] Failed spawning smoke type=" + smokeType + " pos=" + smokePos.ToString());
            return;
        }

        if (smoke.GetCompEM() && smoke.GetCompEM().CanWork())
            smoke.GetCompEM().SwitchOn();

        Print("[EoH_EventObjectives] Spawned event smoke type=" + smokeType + " pos=" + smokePos.ToString());
    }

    void TickRewardCrate()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.RewardCrate)
            return;

        if (!m_ActiveRuntime.RewardCrate.ShouldUnlock())
            return;

        m_ActiveRuntime.RewardCrate.MarkUnlocked();
        m_ActiveRuntime.RewardUnlocked = true;

        SpawnEventSmoke(m_ActiveRuntime.RewardCrate.Position, "M18SmokeGrenade_Green");

        string extractionMsg = "The secured cache at " + m_ActiveRuntime.Config.DisplayName + " is now vulnerable.";
        EoH_Notifications.SendToAll("EVENT EXTRACTION", extractionMsg);

        Print("[EoH_EventObjectives] Reward crate unlocked id=" + m_ActiveRuntime.Config.Id);
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

        EoH_EventObjective cfg = m_ActiveRuntime.Config;

        if (m_ActiveRuntime.SpawnedObject)
            GetGame().ObjectDelete(m_ActiveRuntime.SpawnedObject);

        if (m_ActiveRuntime.RewardCrate)
            m_ActiveRuntime.RewardCrate.Cleanup();

        EoH_MarkerService.RemoveFromAll("EOH_EVENT_" + cfg.Id);
        EoH_MarkerService.RemoveFromAll("EOH_EVENT_INTEL_ALTAR_RELAY");

        string endMsg = cfg.DisplayName + " has gone silent. Intel channels are open again.";
        EoH_Notifications.SendToAll("WEEKEND EVENT", endMsg);

        Print("[EoH_EventObjectives] Ended objective id=" + cfg.Id + " intelAvailable=true");

        m_ActiveRuntime = null;
    }
}
