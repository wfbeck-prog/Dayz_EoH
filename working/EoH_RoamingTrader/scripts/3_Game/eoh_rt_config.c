class EoH_RT_GlobalConfig
{
	private static ref EoH_RT_GlobalConfig s_Instance;
	static const string CONFIG_DIR = "$profile:EoH_RoamingTrader";
	static const string CONFIG_PATH = "$profile:EoH_RoamingTrader/Config.json";

	int Enabled;
	int EnableDebug;
	int AnnounceMoves;
	int ShowMapMarkers;
	int DespawnEscortOnMove;
	int LockTraderWhileMoving;
	int StopDurationMinutes;
	int MoveCheckSeconds;
	int MinPlayersOnline;
	ref array<ref EoH_RT_TraderProfile> Traders;

	void EoH_RT_GlobalConfig()
	{
		Traders = new array<ref EoH_RT_TraderProfile>();
		SetDefaults();
	}

	static EoH_RT_GlobalConfig Get()
	{
		if (!s_Instance)
		{
			s_Instance = new EoH_RT_GlobalConfig();
			s_Instance.Load();
		}
		return s_Instance;
	}

	void SetDefaults()
	{
		Enabled = 1;
		EnableDebug = 1;
		AnnounceMoves = 1;
		ShowMapMarkers = 1;
		DespawnEscortOnMove = 1;
		LockTraderWhileMoving = 1;
		StopDurationMinutes = 30;
		MoveCheckSeconds = 15;
		MinPlayersOnline = 0;
		Traders.Clear();

		EoH_RT_TraderProfile drug = new EoH_RT_TraderProfile();
		drug.TraderId = "eoh_drug_trader";
		drug.DisplayName = "Drug Trader";
		drug.ExpansionTraderUID = "exp_drug_trader";
		drug.MarkerLabel = "Drug Trader";
		drug.AnnouncementTemplate = "Word is the %TRADER% was seen near %LOCATION%.";
		drug.Escort.MinGuards = 2;
		drug.Escort.MaxGuards = 3;
		drug.Escort.AIGroupType = "bandit_light";
		EoH_RT_RouteNode drugNode1 = new EoH_RT_RouteNode();
		drugNode1.Name = "Drug Stop Alpha";
		drugNode1.Position = "7500 0 7500";
		drugNode1.Orientation = 180;
		EoH_RT_RouteNode drugNode2 = new EoH_RT_RouteNode();
		drugNode2.Name = "Drug Stop Bravo";
		drugNode2.Position = "8500 0 6400";
		drugNode2.Orientation = 90;
		drug.Route.Insert(drugNode1);
		drug.Route.Insert(drugNode2);
		Traders.Insert(drug);

		EoH_RT_TraderProfile black = new EoH_RT_TraderProfile();
		black.TraderId = "eoh_black_market";
		black.DisplayName = "Black Market Trader";
		black.ExpansionTraderUID = "exp_black_market";
		black.MarkerLabel = "Black Market Trader";
		black.AnnouncementTemplate = "The %TRADER% has changed location near %LOCATION%.";
		black.Escort.MinGuards = 3;
		black.Escort.MaxGuards = 5;
		black.Escort.AIGroupType = "bandit_heavy";
		EoH_RT_RouteNode blackNode1 = new EoH_RT_RouteNode();
		blackNode1.Name = "Black Market North";
		blackNode1.Position = "4500 0 10200";
		blackNode1.Orientation = 45;
		EoH_RT_RouteNode blackNode2 = new EoH_RT_RouteNode();
		blackNode2.Name = "Black Market South";
		blackNode2.Position = "12100 0 4300";
		blackNode2.Orientation = 270;
		black.Route.Insert(blackNode1);
		black.Route.Insert(blackNode2);
		Traders.Insert(black);
	}

	bool Load()
	{
		if (!FileExist(CONFIG_DIR))
			MakeDirectory(CONFIG_DIR);

		if (!FileExist(CONFIG_PATH))
		{
			Save();
			return true;
		}

		JsonFileLoader<EoH_RT_GlobalConfig>.JsonLoadFile(CONFIG_PATH, this);
		return true;
	}

	bool Save()
	{
		if (!FileExist(CONFIG_DIR))
			MakeDirectory(CONFIG_DIR);
		JsonFileLoader<EoH_RT_GlobalConfig>.JsonSaveFile(CONFIG_PATH, this);
		return true;
	}

	void Log(string msg)
	{
		Print("[EoH_RT] " + msg);
	}

	void DebugLog(string msg)
	{
		if (EnableDebug)
			Print("[EoH_RT][DEBUG] " + msg);
	}

	EoH_RT_TraderProfile FindProfile(string traderId)
	{
		foreach (EoH_RT_TraderProfile profile : Traders)
		{
			if (profile && profile.TraderId == traderId)
				return profile;
		}
		return NULL;
	}
};
