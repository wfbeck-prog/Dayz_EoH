class EoH_WeeklyEventManager
{
    protected static ref EoH_WeeklyEventManager s_Instance;

    static const int STATE_IDLE = 0;
    static const int STATE_WARNING = 1;
    static const int STATE_ACTIVE = 2;
    static const int STATE_SUCCESS = 3;
    static const int STATE_FAILED = 4;

    protected ref EoH_WeeklyEventConfig m_Config;
    protected int m_LastTick;
    protected int m_State;
    protected ref EoH_WeeklyRelayTowerLocation m_ActiveObjective;
    protected int m_StateStart;

    static EoH_WeeklyEventManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_WeeklyEventManager();

        return s_Instance;
    }

    void EoH_WeeklyEventManager()
    {
        m_Config = EoH_WeeklyEventConfig.Load();
        m_LastTick = 0;
        m_State = STATE_IDLE;
        m_ActiveObjective = null;
        m_StateStart = 0;

        Print("[EoH_WeeklyEvents] Manager initialized using standalone event objectives");
    }

    void Tick()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!m_Config || !m_Config.Enabled)
            return;

        int now = GetGame().GetTime();
        if (m_LastTick > 0 && now - m_LastTick < (m_Config.TickSeconds * 1000))
            return;

        m_LastTick = now;

        if (m_State == STATE_IDLE)
            HandleIdle();
        else if (m_State == STATE_WARNING)
            HandleWarning();
        else if (m_State == STATE_ACTIVE)
            HandleActive();
    }

    void HandleIdle()
    {
        if (!m_Config.RelayCollapseEnabled)
            return;

        if (!m_ActiveObjective)
            m_ActiveObjective = PickRandomObjective();

        if (!m_ActiveObjective)
            return;

        StartObjectiveWarning();
    }

    void StartObjectiveWarning()
    {
        if (!m_ActiveObjective)
            return;

        m_State = STATE_WARNING;
        m_StateStart = GetGame().GetTime();

        string msg = "Emergency Broadcast: unusual signal activity detected near " + m_ActiveObjective.DisplayName + ". Investigate immediately.";

        EoH_Notifications.SendToAll("WEEKEND EVENT", msg);
        BroadcastObjectiveMarker("WARNING");

        Print("[EoH_WeeklyEvents] Event warning started objective=" + m_ActiveObjective.Id + " pos=" + m_ActiveObjective.Position.ToString());
    }

    void HandleWarning()
    {
        int elapsed = GetGame().GetTime() - m_StateStart;
        if (elapsed < m_Config.RelayWarningMinutes * 60 * 1000)
            return;

        StartObjectiveActive();
    }

    void StartObjectiveActive()
    {
        if (!m_ActiveObjective)
            return;

        m_State = STATE_ACTIVE;
        m_StateStart = GetGame().GetTime();

        string msg = "Hostile activity escalating near " + m_ActiveObjective.DisplayName + ". High-value objective confirmed.";

        EoH_Notifications.SendToAll("EVENT ACTIVE", msg);
        BroadcastObjectiveMarker("ACTIVE");

        Print("[EoH_WeeklyEvents] Event active objective=" + m_ActiveObjective.Id + " pos=" + m_ActiveObjective.Position.ToString());
    }

    void HandleActive()
    {
        int elapsed = GetGame().GetTime() - m_StateStart;
        if (elapsed < m_Config.RelayActiveMinutes * 60 * 1000)
            return;

        CompleteObjectiveEvent();
    }

    void CompleteObjectiveEvent()
    {
        if (!m_ActiveObjective)
            return;

        m_State = STATE_SUCCESS;

        EoH_Notifications.SendToAll("EVENT ENDED", "Activity near " + m_ActiveObjective.DisplayName + " has subsided.");
        EoH_MarkerService.RemoveFromAll(GetObjectiveMarkerId());

        Print("[EoH_WeeklyEvents] Event completed objective=" + m_ActiveObjective.Id);

        ResetEvent();
    }

    void ResetEvent()
    {
        m_State = STATE_IDLE;
        m_ActiveObjective = null;
        m_StateStart = GetGame().GetTime();
    }

    EoH_WeeklyRelayTowerLocation PickRandomObjective()
    {
        if (!m_Config || !m_Config.EventObjectives || m_Config.EventObjectives.Count() == 0)
            return null;

        return m_Config.EventObjectives.Get(Math.RandomInt(0, m_Config.EventObjectives.Count()));
    }

    string GetObjectiveMarkerId()
    {
        if (!m_ActiveObjective)
            return "EoH_WEEKLY_OBJECTIVE";

        return "EoH_WEEKLY_OBJECTIVE_" + m_ActiveObjective.Id;
    }

    void BroadcastObjectiveMarker(string state)
    {
        if (!m_ActiveObjective)
            return;

        EoH_MarkerData data = new EoH_MarkerData(GetObjectiveMarkerId(), "Event Objective: " + m_ActiveObjective.DisplayName, m_ActiveObjective.Position);
        data.Category = "event";
        data.State = "alert";
        data.Icon = "Radio";
        data.Is3D = 1;
        data.Pulse = 1;
        data.Color = ARGB(255, 255, 120, 0);
        data.BaseColor = data.Color;
        data.Normalize();

        EoH_MarkerService.Broadcast(data);
        Print("[EoH_WeeklyEvents] Broadcast objective marker state=" + state + " id=" + data.Id + " pos=" + data.Position.ToString());
    }
}
