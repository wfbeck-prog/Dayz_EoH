class EoH_RT_RouteNode
{
	string Name;
	vector Position;
	float Orientation;
	int StopDurationMinutes;

	void EoH_RT_RouteNode()
	{
		Name = "";
		Position = "0 0 0";
		Orientation = 0;
		StopDurationMinutes = 30;
	}
};

class EoH_RT_EscortProfile
{
	int Enabled;
	int MinGuards;
	int MaxGuards;
	float GuardRadius;
	string AIGroupType;
	string Faction;
	int RespawnAtEachStop;

	void EoH_RT_EscortProfile()
	{
		Enabled = 1;
		MinGuards = 2;
		MaxGuards = 4;
		GuardRadius = 8.0;
		AIGroupType = "bandit_light";
		Faction = "Bandits";
		RespawnAtEachStop = 1;
	}
};

class EoH_RT_TraderProfile
{
	string TraderId;
	string DisplayName;
	string TraderNPCClassname;
	string ExpansionTraderUID;
	string MarkerLabel;
	string AnnouncementTemplate;
	int Enabled;
	ref array<string> GuardLoadouts;
	ref array<ref EoH_RT_RouteNode> Route;
	ref EoH_RT_EscortProfile Escort;

	void EoH_RT_TraderProfile()
	{
		TraderId = "";
		DisplayName = "Roaming Trader";
		TraderNPCClassname = "EoH_RT_TraderNPC";
		ExpansionTraderUID = "";
		MarkerLabel = "Roaming Trader";
		AnnouncementTemplate = "%TRADER% was seen near %LOCATION%.";
		Enabled = 1;
		GuardLoadouts = new array<string>();
		Route = new array<ref EoH_RT_RouteNode>();
		Escort = new EoH_RT_EscortProfile();
	}
};

class EoH_RT_TraderRuntime
{
	string TraderId;
	int CurrentRouteIndex;
	bool IsSpawned;
	bool IsMoving;
	int LastMoveServerTime;
	Object TraderObject;
	ref array<Object> EscortUnits;
	ref array<Particle> AtmosphereParticles;
	vector CurrentPosition;

	void EoH_RT_TraderRuntime()
	{
		TraderId = "";
		CurrentRouteIndex = 0;
		IsSpawned = false;
		IsMoving = false;
		LastMoveServerTime = 0;
		TraderObject = NULL;
		EscortUnits = new array<Object>();
		CurrentPosition = "0 0 0";
	}
};
