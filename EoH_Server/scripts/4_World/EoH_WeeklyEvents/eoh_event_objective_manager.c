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
        // PRIMARY LIVE EVENT
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

        // SECONDARY FALLBACK EVENT
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
        if (m_ActiveRuntime && m_ActiveRuntime.Active)
        {
            Print("[EoH_EventObjectives] Cannot start objective another is already active");
            return false;
        }

        EoH_EventObjective obj = PickRandomObjective();
        if (!obj)
            return false;

        m_ActiveRuntime = new EoH_EventObjectiveRuntime(obj);
        m_ActiveRuntime.Active = true;
        m_ActiveRuntime.StartTime = GetGame().GetTime();

        SpawnObjectiveObject();
        BroadcastObjective();

        Print("[EoH_EventObjectives] Started objective id=" + obj.Id + " pos=" + obj.Position.ToString());
        return true;
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

    void BroadcastObjective()
    {
        if (!m_ActiveRuntime || !m_ActiveRuntime.Config)
            return;

        EoH_EventObjective cfg = m_ActiveRuntime.Config;

        EoH_Notifications.SendToAll(
            "WEEKEND EVENT",
            "High-value signal activity detected near " + cfg.DisplayName + "."
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

        EoH_MarkerService.RemoveFromAll("EOH_EVENT_" + cfg.Id);

        EoH_Notifications.SendToAll(
            "WEEKEND EVENT",
            cfg.DisplayName + " has gone silent."
        );

        Print("[EoH_EventObjectives] Ended objective id=" + cfg.Id);

        m_ActiveRuntime = null;
    }
}
