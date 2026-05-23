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
    protected string m_ActiveRelayTown;
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
        m_ActiveRelayTown = "";
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

        if (m_ActiveRelayTown == "")
            m_ActiveRelayTown = PickRandomRelayTown();

        if (m_ActiveRelayTown == "")
            return;

        StartRelayWarning();
    }

    void StartRelayWarning()
    {
        m_State = EOH_WEEKLY_EVENT_WARNING;
        m_StateStart = GetGame().GetTime();

        string msg = "Emergency Broadcast: Relay Station near " + m_ActiveRelayTown + " has gone dark. Signal degradation detected across South Zagoria.";

        EoH_Notifications.SendToAll("RELAY COLLAPSE", msg);

        Print("[EoH_WeeklyEvents] Relay Collapse warning started town=" + m_ActiveRelayTown);
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
        m_State = EOH_WEEKLY_EVENT_ACTIVE;
        m_StateStart = GetGame().GetTime();

        string msg = "Relay collapse escalation detected near " + m_ActiveRelayTown + ". Hostile movement reported around the relay sector.";

        EoH_Notifications.SendToAll("RELAY COLLAPSE ACTIVE", msg);

        Print("[EoH_WeeklyEvents] Relay Collapse active town=" + m_ActiveRelayTown);
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
        m_State = EOH_WEEKLY_EVENT_SUCCESS;

        EoH_Notifications.SendToAll("RELAY STABILIZED", "Relay communications near " + m_ActiveRelayTown + " have been restored.");

        Print("[EoH_WeeklyEvents] Relay Collapse completed town=" + m_ActiveRelayTown);

        ResetEvent();
    }

    void ResetEvent()
    {
        m_State = EOH_WEEKLY_EVENT_IDLE;
        m_ActiveRelayTown = "";
        m_StateStart = GetGame().GetTime();
    }

    string PickRandomRelayTown()
    {
        array<string> towns = {
            "Chernogorsk",
            "Elektro",
            "Berezino",
            "Novy Sobor",
            "Stary Sobor",
            "Zelenogorsk",
            "NWAF",
            "Tisy"
        };

        if (towns.Count() == 0)
            return "";

        return towns.Get(Math.RandomInt(0, towns.Count()));
    }
}
