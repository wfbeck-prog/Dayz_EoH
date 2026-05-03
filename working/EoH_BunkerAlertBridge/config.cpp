class CfgPatches
{
    class EoH_BunkerAlertBridge
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;

        // Add the original mod's patch name here too if you know it
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Scripts"
        };
    };
};

class CfgMods
{
    class EoH_BunkerAlertBridge
    {
        dir = "EoH_BunkerAlertBridge";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "EoH_BunkerAlertBridge";
        credits = "OpenAI";
        author = "OpenAI";
        authorID = "0";
        version = "1.0";
        extra = 0;
        type = "mod";

        dependencies[] = { "World" };

        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = { "EoH_BunkerAlertBridge/scripts/4_World" };
            };
        };
    };
};