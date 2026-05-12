modded class DNA_Crate_ColourBase
{
    override void Open()
    {
        super.Open();

        if (!GetGame().IsServer())
            return;

        EoH_DNACacheOpenBridge.OnCrateOpened(this);
    }
};
