modded class DNA_Crate_ColourBase
{
    override void EEInit()
    {
        super.EEInit();

        if (GetGame() && GetGame().IsServer())
            EoH_DNACacheOpenBridge.RegisterLiveCache(this);
    }
}
