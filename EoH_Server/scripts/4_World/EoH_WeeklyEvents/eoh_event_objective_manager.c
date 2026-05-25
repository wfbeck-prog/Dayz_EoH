class EoH_EventObjectiveManager
{
    protected static ref EoH_EventObjectiveManager s_Instance;

    protected ref array<ref EoH_EventObjective> m_Objectives;
    protected ref EoH_EventObjectiveRuntime m_ActiveRuntime;

    static EoH_EventObjectiveManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_EventObjectiveManager();

        return s_Instance;
    }

    void EoH_EventObjectiveManager()
    {
        m_Objectives = new array<ref EoH_EventObjective>();
        RegisterDefaults();

        Print("[EoH_EventObjectives] Manager initialized objectives=" + m_Objectives.Count().ToString());
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
        altarTowers.EnableAIBandits = true;
        altarTowers.EnableInfected = true;
        altarTowers.EnableMarker = true;
        altarTowers.EnableSmoke = true;

        m_Objectives.Insert(altarTowers);

        EoH_EventObjective convoy = new EoH_EventObjective();
        convoy.Id = "convoy_blackmountain";
        convoy.DisplayName = "Destroyed Relay Convoy";
        convoy.Position = "11100 0 2550";
        convoy.ObjectiveType = "convoy";
        convoy.SpawnObject = "Land_Wreck_Ural";
        convoy.RecommendedPlayers = 4;
        convoy.LootTier = 3;

        m_Objectives.Insert(convoy);
    }

    EoH_EventObjective PickRandomObjective()
    {
        if (!m_Objectives || m_Objectives.Count() == 0)
            return null;

        return m_Objectives.Get(Math.RandomInt(0, m_Objectives.Count()));
    }

    bool StartRandomObjective()
    {
        return RevealRandomObjectiveOnly();
    }

    bool RevealRandomObjectiveOnly()
    {
        if (m_ActiveRuntime && m_ActiveRuntime.Active)
        {
            Print("[EoH_EventObjectives] Cannot reveal objective another is already active");
            return false;
        }

        EoH_EventObjective obj = PickRandomObjective();
        if (!obj)
            return false;

        m_ActiveRuntime = new EoH_EventObjectiveRuntime(obj);
        m_ActiveRuntime.Active = true;
        m_ActiveRuntime.StartTime = 0;
        m_ActiveRuntime.LastTickTime = GetGame().GetTime();
        m_ActiveRuntime.RevealedByIntel = true;
        m_ActiveRuntime.RewardCrate = null;
        m_ActiveRuntime.CurrentWave = 0;

        SpawnObjectiveObject();
        BroadcastObjective();

        Print("[EoH_EventObjectives] Revealed objective id=" + obj.Id + " awaiting field repair pos=" + obj.Position.ToString());
        return true;
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
        EoH_EventWaveManager.Get().SpawnWave(m_ActiveRuntime, 1);
        m_ActiveRuntime.CurrentWave = 1;

        EoH_Notifications.SendToAll(
            "RELAY ONLINE",
            cfg.DisplayName + " has been restored. Hostile contact is moving toward the signal."
        );

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
        TickWaves(now);
        TickRewardCrate();
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

        if (cfg.EnableSmoke)
        {
            Particle.PlayInWorld(ParticleList.SMOKEGRENADE_RED, cfg.Position);
        }
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

    void TickRewardCrate()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.RewardCrate)
            return;

        if (!m_ActiveRuntime.RewardCrate.ShouldUnlock())
            return;

        m_ActiveRuntime.RewardCrate.MarkUnlocked();
        m_ActiveRuntime.RewardUnlocked = true;

        EoH_Notifications.SendToAll(
            "EVENT EXTRACTION",
            "The secured cache at " + m_ActiveRuntime.Config.DisplayName + " is now vulnerable."
        );

        Print("[EoH_EventObjectives] Reward crate unlocked id=" + m_ActiveRuntime.Config.Id);
    }

    void BroadcastObjective()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;

        EoH_EventObjective cfg = m_ActiveRuntime.Config;

        EoH_Notifications.SendToAll(
            "WEEKEND EVENT",
            "High-value signal activity detected near " + cfg.DisplayName + ". Field repair required."
        );

        if (!cfg.EnableMarker)
            return;

        EoH_MarkerData data = new EoH_MarkerData(
            "EOH_EVENT_" + cfg.Id,
            cfg.DisplayName,
            cfg.Position
        );

        data.Category = "event";
        data.Icon = "Radio";
        data.Is3D = 1;
        data.Pulse = 1;
        data.Color = ARGB(255, 255, 80, 0);
        data.BaseColor = data.Color;
        data.Normalize();

        EoH_MarkerService.Broadcast(data);
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

        EoH_Notifications.SendToAll(
            "WEEKEND EVENT",
            cfg.DisplayName + " has gone silent."
        );

        Print("[EoH_EventObjectives] Ended objective id=" + cfg.Id);

        m_ActiveRuntime = null;
    }
}
