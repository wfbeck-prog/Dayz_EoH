enum EoH_WeeklyEventState
{
    EOH_WEEKLY_EVENT_IDLE = 0,
    EOH_WEEKLY_EVENT_WARNING,
    EOH_WEEKLY_EVENT_ACTIVE,
    EOH_WEEKLY_EVENT_SUCCESS,
    EOH_WEEKLY_EVENT_FAILED
}

class EoH_WeeklyEventManager
{
    protected static ref EoH_WeeklyEventManager s_Instance;

    protected ref EoH_WeeklyEventConfig m_Config;
    protected int m_LastTick;
    protected int m_State;
    protected ref EoH_WeeklyRelayTowerLocation m_ActiveRelayTower;
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
        m_State = EOH_WEEKLY_EVENT_IDLE;
        m_ActiveRelayTower = null;
        m_StateStart = 0;

        Print("[EoH_WeeklyEvents] Manager initialized");
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

        switch (m_State)
        {
            case EOH_WEEKLY_EVENT_IDLE:
                HandleIdle();
                break;

            case EOH_WEEKLY_EVENT_WARNING:
                HandleWarning();
                break;

            case EOH_WEEKLY_EVENT_ACTIVE:
                HandleActive();
                break;
        }
    }

    void HandleIdle()
    {
        if (!m_Config.RelayCollapseEnabled)
            return;

        if (!m_ActiveRelayTower)
            m_ActiveRelayTower = PickRandomRelayTower();

        if (!m_ActiveRelayTower)
            return;

        StartRelayWarning();
    }

    void StartRelayWarning()
    {
        if (!m_ActiveRelayTower)
            return;

        m_State = EOH_WEEKLY_EVENT_WARNING;
        m_StateStart = GetGame().GetTime();

        string msg = "Emergency Broadcast: " + m_ActiveRelayTower.DisplayName + " has gone dark. Signal degradation detected across South Zagoria.";

        EoH_Notifications.SendToAll("RELAY COLLAPSE", msg);
        BroadcastRelayMarker("WARNING");

        Print("[EoH_WeeklyEvents] Relay Collapse warning started tower=" + m_ActiveRelayTower.Id + " pos=" + m_ActiveRelayTower.Position.ToString());
    }

    void HandleWarning()
    {
        int elapsed = GetGame().GetTime() - m_StateStart;
        if (elapsed < m_Config.RelayWarningMinutes * 60 * 1000)
            return;

        StartRelayActive();
    }

    void StartRelayActive()
    {
        if (!m_ActiveRelayTower)
            return;

        m_State = EOH_WEEKLY_EVENT_ACTIVE;
        m_StateStart = GetGame().GetTime();

        string msg = "Relay collapse escalation detected at " + m_ActiveRelayTower.DisplayName + ". Hostile movement reported around the relay sector.";

        EoH_Notifications.SendToAll("RELAY COLLAPSE ACTIVE", msg);
        BroadcastRelayMarker("ACTIVE");

        Print("[EoH_WeeklyEvents] Relay Collapse active tower=" + m_ActiveRelayTower.Id + " pos=" + m_ActiveRelayTower.Position.ToString());
    }

    void HandleActive()
    {
        int elapsed = GetGame().GetTime() - m_StateStart;
        if (elapsed < m_Config.RelayActiveMinutes * 60 * 1000)
            return;

        CompleteRelayEvent();
    }

    void CompleteRelayEvent()
    {
        if (!m_ActiveRelayTower)
            return;

        m_State = EOH_WEEKLY_EVENT_SUCCESS;

        EoH_Notifications.SendToAll("RELAY STABILIZED", "Relay communications at " + m_ActiveRelayTower.DisplayName + " have been restored.");
        EoH_MarkerService.RemoveFromAll(GetRelayMarkerId());

        Print("[EoH_WeeklyEvents] Relay Collapse completed tower=" + m_ActiveRelayTower.Id);

        ResetEvent();
    }

    void ResetEvent()
    {
        m_State = EOH_WEEKLY_EVENT_IDLE;
        m_ActiveRelayTower = null;
        m_StateStart = GetGame().GetTime();
    }

    EoH_WeeklyRelayTowerLocation PickRandomRelayTower()
    {
        if (!m_Config || !m_Config.RelayTowers || m_Config.RelayTowers.Count() == 0)
            return null;

        return m_Config.RelayTowers.Get(Math.RandomInt(0, m_Config.RelayTowers.Count()));
    }

    string GetRelayMarkerId()
    {
        if (!m_ActiveRelayTower)
            return "EoH_RELAY_COLLAPSE";

        return "EoH_RELAY_COLLAPSE_" + m_ActiveRelayTower.Id;
    }

    void BroadcastRelayMarker(string state)
    {
        if (!m_ActiveRelayTower)
            return;

        EoH_MarkerData data = new EoH_MarkerData(GetRelayMarkerId(), "Relay Collapse: " + m_ActiveRelayTower.DisplayName, m_ActiveRelayTower.Position);
        data.Category = EoH_MarkerCategory.TOWN;
        data.State = EoH_MarkerState.ALERT;
        data.Icon = "Radio";
        data.Is3D = 1;
        data.Pulse = 1;
        data.Color = ARGB(255, 255, 120, 0);
        data.BaseColor = data.Color;
        data.Normalize();

        EoH_MarkerService.Broadcast(data);
        Print("[EoH_WeeklyEvents] Broadcast relay event marker state=" + state + " id=" + data.Id + " pos=" + data.Position.ToString());
    }
}
