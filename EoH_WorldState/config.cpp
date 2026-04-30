class CfgPatches
{
    class EoH_WorldState
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data"};
    };
};

class CfgMods
{
    class EoH_WorldState
    {
        dir = "EoH_WorldState";
        picture = "";
        action = "";
        hideName = 0;
        hidePicture = 1;
        name = "EoH World State";
        credits = "Echoes of Humanity";
        author = "Echoes of Humanity";
        authorID = "0";
        version = "0.1";
        extra = 0;
        type = "mod";

        dependencies[] = {"World", "Mission"};

        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"EoH_WorldState/scripts/4_World"};
            };

            class missionScriptModule
            {
                value = "";
                files[] = {"EoH_WorldState/scripts/5_Mission"};
            };
        };
    };
};
