modded class MissionServer
{
    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        // Centralized EoH scheduler heartbeat.
        // All future EoH systems should migrate here instead of creating independent Tick loops.
        EoH_Scheduler.Get().Tick();

        // Disabled temporarily for live-server performance recovery.
        // This scans TerritoryFlag objects and should be reworked to use cached flag registration.
        // EoH_GroupFlagMarkerManager.Tick();

        // Disabled for live-server performance recovery.
        // The DNA relocator currently performs expensive world object scans and caused severe FPS drops.
        // Re-enable only after converting it to cached crate registration instead of 30km scans.
        // EoH_DNACrateRelocator.Get().Tick();
    }
}
