class CfgPatches
{
    class EoH_Server
    {
        units[] =
        {
            "EoH_CaptureRelay_Base",
            "EoH_RadioRelay",
            "EoH_RelayRepairTerminal",
            "EoH_RelayRepairActionAnchor",
            "EoH_Intel_Document",
            "EoH_TownIntel",
            "EoH_TraderIntel",
            "EoH_CBDIntel",
            "EoH_LootRoomIntel",
            "EoH_AltarRelayIntel",
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
    class Radio;
    class ItemBook;
    class bldr_prop_BaseRadio;
    class Inventory_Base;
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

    class EoH_RelayRepairTerminal: bldr_prop_BaseRadio
    {
        scope = 2;
        displayName = "EoH Relay Repair Terminal";
        descriptionShort = "A damaged relay terminal used to restore the Altar tower network.";
    };

    class EoH_RelayRepairActionAnchor: Inventory_Base
    {
        scope = 2;
        displayName = "Relay Repair Interface";
        descriptionShort = "Hidden EoH interaction anchor for the Altar relay terminal.";
        model = "\dz\gear\radio\unitra_wilga.p3d";
        weight = 100000;
        itemSize[] = {10,10};
    };

    class EoH_Intel_Document: ItemBook
    {
        scope = 2;
        displayName = "Intel Document";
        descriptionShort = "Contains real reconnaissance data about territorial control.";
        model = "\dz\gear\books\book_kniga.p3d";
        rotationFlags = 1;
        itemSize[] = {1,2};
        weight = 100;
    };

    class EoH_TownIntel: EoH_Intel_Document
    {
        scope = 2;
        displayName = "Town Intel";
        descriptionShort = "Reveals nearby town control intel.";
    };

    class EoH_TraderIntel: EoH_Intel_Document
    {
        scope = 2;
        displayName = "Trader Intel";
        descriptionShort = "Contains information about roaming trader activity.";
    };

    class EoH_CBDIntel: EoH_Intel_Document
    {
        scope = 2;
        displayName = "Loot Room Intel";
        descriptionShort = "A damaged lockroom ledger containing partial sealed-room coordinates.";
    };

    class EoH_LootRoomIntel: EoH_CBDIntel
    {
        scope = 2;
        displayName = "Loot Cache Intel";
        descriptionShort = "Recovered notes pointing toward sealed loot caches.";
    };

    class EoH_AltarRelayIntel: EoH_Intel_Document
    {
        scope = 2;
        displayName = "Altar Relay Intel";
        descriptionShort = "Encrypted EoH signal notes identifying the Altar relay array and the field repair parts required to restore it.";
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
