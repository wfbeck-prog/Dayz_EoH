modded class DNA_Crate_ColourBase
{
    override void Open()
    {
        super.Open();

        if (!GetGame().IsServer())
            return;

        Print("[EoH_DNAHook] DNA_Crate_ColourBase.Open fired type=" + GetType() + " pos=" + GetPosition().ToString());
        EoH_DNACacheOpenBridge.OnCrateOpened(this);
    }
};
