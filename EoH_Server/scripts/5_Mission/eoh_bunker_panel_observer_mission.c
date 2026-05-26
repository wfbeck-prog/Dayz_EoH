modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();

        if (EoH_LiveAdvisorLogger.m_Config && EoH_LiveAdvisorLogger.m_Config.EnableBunkerPanelObserver)
        {
            EoH_BunkerPanelObserver.Get().Start();
            Print("[EoH_BunkerObserver] MissionServer OnInit hook registered");
        }
        else
        {
            Print("[EoH_BunkerObserver] Observer disabled in Live Advisor config");
        }
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        if (EoH_LiveAdvisorLogger.m_Config && EoH_LiveAdvisorLogger.m_Config.EnableBunkerPanelObserver)
        {
            EoH_BunkerPanelObserver.Get().Update(timeslice);
        }
    }
}
