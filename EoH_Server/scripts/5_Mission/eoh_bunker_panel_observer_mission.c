modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();

        EoH_BunkerPanelObserver.Get().Start();
        Print("[EoH_BunkerObserver] MissionServer OnInit hook registered");
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        EoH_BunkerPanelObserver.Get().Update(timeslice);
    }
}
