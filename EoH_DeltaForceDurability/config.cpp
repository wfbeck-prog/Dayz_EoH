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
            "DZ_Characters_Headgear",
            "DZ_Characters_Glasses",
            "DZ_Characters_Vests",
            "DZ_Sounds_Effects",
            "My_DF_Gear_Scripts"
        };
    };
};

class CfgVehicles
{
    class My_DF_Gear_Rigs_Mounting;
    class My_DF_Gear_Rigs_Nylon;
    class My_DF_Gear_Rigs_OperationDragon;
    class My_DF_Gear_Rigs_Pilot;
    class My_DF_Gear_Rigs_Pocket;
    class My_DF_Gear_Rigs_Raider;
    class My_DF_Gear_Rigs_Rapid;
    class My_DF_Gear_Rigs_Reis;
    class My_DF_Gear_Rigs_Rocketeer;
    class My_DF_Gear_Rigs_Simple;
    class My_DF_Gear_Rigs_Sineva;
    class My_DF_Gear_Rigs_Sineva_Haavk;
    class My_DF_Gear_Rigs_Universal;

    class My_DF_Gear_Rigs_Mounting { class DamageSystem { class GlobalHealth { class Health { hitpoints = 200; transferToAttachmentsCoef = 0.35; }; }; }; };
    class My_DF_Gear_Rigs_Nylon { class DamageSystem { class GlobalHealth { class Health { hitpoints = 200; transferToAttachmentsCoef = 0.35; }; }; }; };
    class My_DF_Gear_Rigs_OperationDragon { class DamageSystem { class GlobalHealth { class Health { hitpoints = 200; transferToAttachmentsCoef = 0.35; }; }; }; };
    class My_DF_Gear_Rigs_Pilot { class DamageSystem { class GlobalHealth { class Health { hitpoints = 200; transferToAttachmentsCoef = 0.35; }; }; }; };
    class My_DF_Gear_Rigs_Pocket { class DamageSystem { class GlobalHealth { class Health { hitpoints = 200; transferToAttachmentsCoef = 0.35; }; }; }; };
    class My_DF_Gear_Rigs_Raider { class DamageSystem { class GlobalHealth { class Health { hitpoints = 200; transferToAttachmentsCoef = 0.35; }; }; }; };
    class My_DF_Gear_Rigs_Rapid { class DamageSystem { class GlobalHealth { class Health { hitpoints = 200; transferToAttachmentsCoef = 0.35; }; }; }; };
    class My_DF_Gear_Rigs_Reis { class DamageSystem { class GlobalHealth { class Health { hitpoints = 200; transferToAttachmentsCoef = 0.35; }; }; }; };
    class My_DF_Gear_Rigs_Rocketeer { class DamageSystem { class GlobalHealth { class Health { hitpoints = 200; transferToAttachmentsCoef = 0.35; }; }; }; };
    class My_DF_Gear_Rigs_Simple { class DamageSystem { class GlobalHealth { class Health { hitpoints = 200; transferToAttachmentsCoef = 0.35; }; }; }; };
    class My_DF_Gear_Rigs_Sineva { class DamageSystem { class GlobalHealth { class Health { hitpoints = 200; transferToAttachmentsCoef = 0.35; }; }; }; };
    class My_DF_Gear_Rigs_Sineva_Haavk { class DamageSystem { class GlobalHealth { class Health { hitpoints = 200; transferToAttachmentsCoef = 0.35; }; }; }; };
    class My_DF_Gear_Rigs_Universal { class DamageSystem { class GlobalHealth { class Health { hitpoints = 200; transferToAttachmentsCoef = 0.35; }; }; }; };
};
