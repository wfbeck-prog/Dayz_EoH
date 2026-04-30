modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();

        EoH_WorldStateManager.Get();
        Print("[EoH_WorldState] Initialized");
    }
};
