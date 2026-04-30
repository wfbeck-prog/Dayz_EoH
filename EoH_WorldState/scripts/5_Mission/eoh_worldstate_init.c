modded class MissionServer
{
    protected int m_EoH_LastCaptureTick;

    override void OnInit()
    {
        super.OnInit();

        EoH_WorldStateManager.Get();
        EoH_CaptureManager.Get();

        m_EoH_LastCaptureTick = 0;

        Print("[EoH_WorldState] Initialized");
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        int now = GetGame().GetTime();

        // Run capture tick every 5 seconds
        if (now - m_EoH_LastCaptureTick > 5000)
        {
            EoH_CaptureManager.Get().Tick();
            m_EoH_LastCaptureTick = now;
        }
    }
};
