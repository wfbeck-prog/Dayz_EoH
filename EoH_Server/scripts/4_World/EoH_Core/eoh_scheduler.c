class EoH_SchedulerTask
{
    string Id;
    int IntervalMs;
    int LastRun;

    void EoH_SchedulerTask(string id, int intervalMs)
    {
        Id = id;
        IntervalMs = intervalMs;
        LastRun = 0;
    }

    bool ShouldRun(int now)
    {
        if (LastRun <= 0)
            return true;

        return (now - LastRun) >= IntervalMs;
    }

    void MarkRun(int now)
    {
        LastRun = now;
    }
}

class EoH_Scheduler
{
    protected static ref EoH_Scheduler s_Instance;

    protected ref map<string, ref EoH_SchedulerTask> m_Tasks;
    protected int m_LastHeartbeat;

    static const int HEARTBEAT_INTERVAL_MS = 5000;

    static EoH_Scheduler Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_Scheduler();

        return s_Instance;
    }

    void EoH_Scheduler()
    {
        m_Tasks = new map<string, ref EoH_SchedulerTask>();
        m_LastHeartbeat = 0;

        RegisterTask("weekly_events", 60000);
        RegisterTask("town_ai", 60000);
        RegisterTask("town_capture", 30000);
        RegisterTask("group_markers", 120000);

        Print("[EoH_Scheduler] Initialized centralized scheduler");
    }

    void RegisterTask(string id, int intervalMs)
    {
        if (id == "")
            return;

        if (m_Tasks.Contains(id))
            return;

        m_Tasks.Set(id, new EoH_SchedulerTask(id, intervalMs));
        Print("[EoH_Scheduler] Registered task id=" + id + " intervalMs=" + intervalMs.ToString());
    }

    void Tick()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        int now = GetGame().GetTime();
        if (m_LastHeartbeat > 0 && now - m_LastHeartbeat < HEARTBEAT_INTERVAL_MS)
            return;

        m_LastHeartbeat = now;

        RunTask("weekly_events", now);
        RunTask("town_ai", now);
        RunTask("town_capture", now);
    }

    void RunTask(string id, int now)
    {
        EoH_SchedulerTask task;
        if (!m_Tasks.Find(id, task) || !task)
            return;

        if (!task.ShouldRun(now))
            return;

        task.MarkRun(now);

        if (id == "weekly_events")
        {
            EoH_WeeklyEventManager.Get().Tick();
        }
        else if (id == "town_ai")
        {
            EoH_TownAIManager.Get().Tick();
        }
        else if (id == "town_capture")
        {
            EoH_CaptureManager.Get().Tick();
        }

        Print("[EoH_Scheduler] Executed task=" + id + " at=" + now.ToString());
    }
}
