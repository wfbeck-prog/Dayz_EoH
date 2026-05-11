class CfgPatches
{
    class EoH_DeltaForceDurability
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Characters",
            "DZ_Characters_Backpacks",
            "My_DF_Gear_Backpacks_Shiled2"
        };
    };
};

class CfgVehicles
{
    class My_DF_Gear_Backpacks_Shiled2;

    class My_DF_Gear_Backpacks_Shiled2
    {
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints = 875;
                    transferToAttachmentsCoef = 0.35;
                };
            };
        };
    };
};
