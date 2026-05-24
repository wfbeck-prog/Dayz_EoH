modded class MissionServer
{
    protected int m_EoH_HeartbeatLastRun;
    protected const int EOH_HEARTBEAT_INTERVAL_MS = 60000;

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        int now = GetGame().GetTime();
        if (m_EoH_HeartbeatLastRun > 0 && now - m_EoH_HeartbeatLastRun < EOH_HEARTBEAT_INTERVAL_MS)
            return;

        m_EoH_HeartbeatLastRun = now;

        EoH_WeeklyEventManager.Get().Tick();

        // Disabled temporarily for live-server performance recovery.
        // This scans TerritoryFlag objects and should be reworked to use cached flag registration.
        // EoH_GroupFlagMarkerManager.Tick();

        // Disabled for live-server performance recovery.
        // The DNA relocator currently performs expensive world object scans and caused severe FPS drops.
        // Re-enable only after converting it to cached crate registration instead of 30km scans.
        // EoH_DNACrateRelocator.Get().Tick();
    }
}
