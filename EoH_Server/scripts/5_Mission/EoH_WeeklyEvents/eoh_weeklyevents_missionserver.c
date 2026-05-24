modded class MissionServer
{
    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        EoH_WeeklyEventManager.Get().Tick();
        EoH_GroupFlagMarkerManager.Tick();

        // Disabled for live-server performance recovery.
        // The DNA relocator currently performs expensive world object scans and caused severe FPS drops.
        // Re-enable only after converting it to cached crate registration instead of 30km scans.
        // EoH_DNACrateRelocator.Get().Tick();
    }
}
