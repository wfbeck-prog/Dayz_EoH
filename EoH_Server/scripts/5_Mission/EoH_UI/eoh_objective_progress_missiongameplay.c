modded class MissionGameplay
{
    override void OnInit()
    {
        super.OnInit();

        EoH_ObjectiveProgressClient.Get();
        Print("[EoH_ObjectiveProgress] Client initialized from MissionGameplay");
    }
}
