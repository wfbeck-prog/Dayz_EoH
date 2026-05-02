class CfgPatches
{
    class EoH_DogtagBridge
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Scripts",
            "Dogtags_Scripts"
        };
    };
};

class CfgMods
{
    class EoH_DogtagBridge
    {
        dir = "EoH_DogtagBridge";
        name = "EoH Dogtag Bridge";
        type = "mod";

        dependencies[] = {"World", "Mission"};

        class defs
        {
            class worldScriptModule
            {
                files[] = {"EoH_DogtagBridge/scripts/4_World"};
            };

            class missionScriptModule
            {
                files[] = {"EoH_DogtagBridge/scripts/5_Mission"};
            };
        };
    };
};
