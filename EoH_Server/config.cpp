class CfgPatches
{
    class EoH_Server
    {
        units[] = {"EoH_CaptureRelay_Base"};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Scripts",
            "CF",
            "DayZExpansion_Core",
            "DayZExpansion_Navigation",
            "DayZExpansion_Groups",
            "Dogtags",
            "TerjeSkills"
        };
    };
};

class CfgMods
{
    class EoH_Server
    {
        dir = "EoH_Server";
        picture = "";
        action = "";
        hideName = 0;
        hidePicture = 1;
        name = "Echoes of Humanity Server";
        credits = "Echoes of Humanity";
        author = "Echoes of Humanity";
        authorID = "0";
        version = "0.1";
        extra = 0;
        type = "mod";

        dependencies[] = {"Game", "World", "Mission"};

        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] =
                {
                    "EoH_Server/scripts/3_Game"
                };
            };

            class worldScriptModule
            {
                value = "";
                files[] =
                {
                    "EoH_Server/scripts/4_World"
                };
            };

            class missionScriptModule
            {
                value = "";
                files[] =
                {
                    "EoH_Server/scripts/5_Mission"
                };
            };
        };
    };
};

class CfgVehicles
{
    class Inventory_Base;

    class EoH_CaptureRelay_Base: Inventory_Base
    {
        scope = 2;
        displayName = "EoH Radio Relay";
        descriptionShort = "A damaged radio relay used to project group influence across the region.";
        model = "\dz\gear\radio\unitra_wilga.p3d";
        weight = 5000;
        itemSize[] = {4, 4};
        physLayer = "item_large";
    };
};
