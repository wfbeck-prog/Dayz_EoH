class CfgPatches
{
    class EoH_Server
    {
        units[] =
        {
            "EoH_CaptureRelay_Base",
            "EoH_RadioRelay",
            "EoH_Intel_Document",
            "EoH_TownIntel",
            "EoH_TraderIntel",
            "ExpansionTraderChem",
            "ExpansionTraderBroker"
        };
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Scripts",
            "DZ_Gear_Camping",
            "DZ_Gear_Books",
            "DZ_Gear_Consumables",
            "CF",
            "DayZExpansion_Core",
            "DayZExpansion_Navigation",
            "DayZExpansion_Groups",
            "DayZExpansion_Market",
            "Dogtags",
            "TerjeSkills",
            "CBD_LootRoomsSystem"
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
    class Radio;
    class BookAVoyageToArcturus;
    class BookDeadSouls;
    class ExpansionTraderMirek;
    class ExpansionTraderBoris;

class EoH_CaptureRelay_Base: Radio
{
    scope = 2;
    displayName = "EoH Radio Relay";
    descriptionShort = "A modified radio used to project group influence across the region.";
};

class EoH_RadioRelay: EoH_CaptureRelay_Base
{
    scope = 2;
    displayName = "EoH Radio Relay";
    descriptionShort = "A modified radio used to broadcast nearby town intel.";
};

    class EoH_Intel_Document: BookAVoyageToArcturus
    {
        scope = 2;
        displayName = "Intel Document";
        descriptionShort = "Contains real reconnaissance data about territorial control.";
    };

    class EoH_TownIntel: EoH_Intel_Document
    {
        scope = 2;
        displayName = "Town Intel";
        descriptionShort = "Reveals nearby town control intel.";
    };

    class EoH_TraderIntel: BookDeadSouls
    {
        scope = 2;
        displayName = "Trader Intel";
        descriptionShort = "Contains information about roaming trader activity.";
    };

    class ExpansionTraderChem: ExpansionTraderMirek
    {
        scope = 2;
        displayName = "Expansion Trader Chem";
    };

    class ExpansionTraderBroker: ExpansionTraderBoris
    {
        scope = 2;
        displayName = "Expansion Trader Broker";
    };
};
