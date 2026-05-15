modded class DNA_Crate_ColourBase
{
    override void EEInit()
    {
        super.EEInit();

        if (GetGame() && GetGame().IsServer())
            EoH_DNACacheOpenBridge.RegisterLiveCache(this);
    }
}

modded class DNA_Crate_ColorBase
{
    override void EEInit()
    {
        super.EEInit();

        if (GetGame() && GetGame().IsServer())
            EoH_DNACacheOpenBridge.RegisterLiveCache(this);
    }
}

modded class dna_crate_colourbase
{
    override void EEInit()
    {
        super.EEInit();

        if (GetGame() && GetGame().IsServer())
            EoH_DNACacheOpenBridge.RegisterLiveCache(this);
    }
}
