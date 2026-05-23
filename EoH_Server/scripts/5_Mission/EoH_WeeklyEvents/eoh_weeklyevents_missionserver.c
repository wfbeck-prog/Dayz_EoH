modded class MissionServer
{
    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        EoH_WeeklyEventManager.Get().Tick();
        EoH_GroupFlagMarkerManager.Tick();
    }
}
