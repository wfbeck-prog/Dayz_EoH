modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();

        if (GetGame() && GetGame().IsServer())
        {
            EoH_TownAIManager.Get();
            Print("[EoH_TownAI] Mission startup initialization requested.");
        }
    }
};
