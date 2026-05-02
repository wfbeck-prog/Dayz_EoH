class CfgPatches
{
	class EoH_RoamingTrader
	{
		units[] = {"ExpansionTraderChem", "ExpansionTraderBroker"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {
			"DZ_Data",
	"DayZExpansion_Navigation",
	"CBD_LootRoomsSystem"
	};
	};
};

class CfgMods
{
	class EoH_RoamingTrader
	{
		dir = "EoH_RoamingTrader";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 0;
		name = "EoH Roaming Trader";
		credits = "EoH";
		author = "EoH";
		authorID = "0";
		version = "1.0";
		extra = 0;
		type = "mod";

		dependencies[] = {"Game", "World", "Mission"};

		class defs
		{
			class gameScriptModule
			{
				files[] = {"EoH_RoamingTrader/scripts/3_Game"};
			};
			class worldScriptModule
			{
				files[] = {"EoH_RoamingTrader/scripts/4_World"};
			};
			class missionScriptModule
			{
				files[] = {"EoH_RoamingTrader/scripts/5_Mission"};
			};
		};
	};
};

class CfgVehicles
{
	class ExpansionTraderMirek;
	class ExpansionTraderBoris;

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