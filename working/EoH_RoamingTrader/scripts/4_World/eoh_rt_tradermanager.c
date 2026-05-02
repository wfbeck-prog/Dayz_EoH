class EoH_RT_TraderManager
{
	private static ref EoH_RT_TraderManager s_Instance;
	protected ref EoH_RT_GlobalConfig m_Config;
	protected ref map<string, ref EoH_RT_TraderRuntime> m_Runtimes;
	protected ref map<string, bool> m_RevealedMarkers;
	protected bool m_Initialized;

	static EoH_RT_TraderManager GetInstance()
	{
		if (!s_Instance)
			s_Instance = new EoH_RT_TraderManager();
		return s_Instance;
	}

	void EoH_RT_TraderManager()
	{
		m_Config = NULL;
		m_Runtimes = new map<string, ref EoH_RT_TraderRuntime>();
		m_RevealedMarkers = new map<string, bool>();
		m_Initialized = false;
	}

	void Initialize()
	{
		if (m_Initialized)
			return;

		m_Config = EoH_RT_GlobalConfig.Get();
		if (!m_Config)
		{
			Print("[EoH_RT][ERROR] Failed to load config");
			return;
		}

		foreach (EoH_RT_TraderProfile profile : m_Config.Traders)
		{
			if (!profile || !profile.Enabled)
				continue;

			EoH_RT_TraderRuntime runtime = new EoH_RT_TraderRuntime();
			runtime.TraderId = profile.TraderId;
			runtime.CurrentRouteIndex = 0;
			runtime.LastMoveServerTime = GetGame().GetTime();
			runtime.IsSpawned = false;
			runtime.TraderObject = NULL;

			if (!runtime.EscortUnits)
				runtime.EscortUnits = new array<Object>();

			m_Runtimes.Set(profile.TraderId, runtime);
			m_RevealedMarkers.Set(profile.TraderId, false);
		}

		m_Initialized = true;
		Print("[EoH_RT] Initialized");
	}

	void Update()
	{
		if (!m_Initialized || !m_Config || !m_Config.Enabled)
			return;

		foreach (string id, EoH_RT_TraderRuntime runtime : m_Runtimes)
		{
			EoH_RT_TraderProfile profile = m_Config.FindProfile(id);
			if (!profile)
				continue;

			if (!runtime.IsSpawned || !runtime.TraderObject)
			{
				SpawnTraderAtCurrentNode(runtime);
				continue;
			}

			if (ShouldAdvanceRoute(profile, runtime))
				MoveTraderToNextNode(profile, runtime);
		}
	}

	bool ShouldAdvanceRoute(EoH_RT_TraderProfile profile, EoH_RT_TraderRuntime runtime)
	{
		EoH_RT_RouteNode node = EoH_RT_TraderRouteSystem.GetCurrentNode(profile, runtime);
		if (!node)
			return false;

		int stopMinutes = node.StopDurationMinutes;
		if (stopMinutes <= 0)
			stopMinutes = m_Config.StopDurationMinutes;

		int elapsed = GetGame().GetTime() - runtime.LastMoveServerTime;
		return elapsed >= stopMinutes * 60 * 1000;
	}

	void SpawnTraderAtCurrentNode(EoH_RT_TraderRuntime runtime)
	{
		if (!runtime)
			return;

		EoH_RT_TraderProfile profile = m_Config.FindProfile(runtime.TraderId);
		if (!profile || !profile.Route || profile.Route.Count() == 0)
			return;

		if (runtime.CurrentRouteIndex < 0 || runtime.CurrentRouteIndex >= profile.Route.Count())
			runtime.CurrentRouteIndex = 0;

		EoH_RT_RouteNode node = profile.Route.Get(runtime.CurrentRouteIndex);
		if (!node)
			return;

		if (!runtime.TraderObject)
		{
			runtime.TraderObject = FindExistingTraderObject(profile.TraderNPCClassname);
			if (!runtime.TraderObject)
			{
				Print("[EoH_RT][ERROR] Could not find registered trader object for " + profile.TraderNPCClassname);
				return;
			}
		}

		PlaceTraderObject(runtime.TraderObject, node.Position, node.Orientation);

		runtime.IsSpawned = true;
		runtime.LastMoveServerTime = GetGame().GetTime();

		EoH_RT_AIIntegration.CleanupEscort(runtime);
		EoH_RT_AIIntegration.SpawnEscort(profile, runtime, runtime.TraderObject.GetPosition());

		//! EoH_RT_Atmosphere.CleanupAtmosphere(runtime);
		//! EoH_RT_Atmosphere.SpawnAtmosphere(profile, runtime, runtime.TraderObject.GetPosition());

		//! Do NOT auto-broadcast marker on spawn
		m_RevealedMarkers.Set(profile.TraderId, false);

		Print("[EoH_RT] Positioned trader " + profile.TraderId + " at " + runtime.TraderObject.GetPosition().ToString());
	}

	Object FindExistingTraderObject(string className)
	{
		array<Object> objects = new array<Object>();
		array<CargoBase> proxy = new array<CargoBase>();

		GetGame().GetObjectsAtPosition3D("7500 0 7500", 30000, objects, proxy);

		for (int i = 0; i < objects.Count(); i++)
		{
			Object obj = objects.Get(i);
			if (!obj)
				continue;

			if (obj.GetType() != className)
				continue;

			if (IsTraderObjectAlreadyClaimed(obj))
				continue;

			return obj;
		}

		return NULL;
	}

	bool IsTraderObjectAlreadyClaimed(Object obj)
	{
		foreach (string id, EoH_RT_TraderRuntime runtime : m_Runtimes)
		{
			if (runtime && runtime.TraderObject == obj)
				return true;
		}
		return false;
	}

	void PlaceTraderObject(Object traderObj, vector rawPos, float yaw)
	{
		if (!traderObj)
			return;

		vector pos = rawPos;
		pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.18;

		traderObj.SetPosition(pos);
		traderObj.SetOrientation("0 0 0");

		vector ori = "0 0 0";
		ori[0] = yaw;
		traderObj.SetOrientation(ori);

		pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.18;
		traderObj.SetPosition(pos);
	}

	EoH_RT_TraderRuntime GetRuntimeByObject(Object obj)
	{
		foreach (string id, EoH_RT_TraderRuntime runtime : m_Runtimes)
		{
			if (runtime && runtime.TraderObject == obj)
				return runtime;
		}
		return NULL;
	}

	void MoveTraderToNextNode(EoH_RT_TraderProfile profile, EoH_RT_TraderRuntime runtime)
	{
		if (!profile || !runtime || !runtime.TraderObject)
			return;

		if (m_Config.DespawnEscortOnMove)
			EoH_RT_AIIntegration.CleanupEscort(runtime);

		//! EoH_RT_Atmosphere.CleanupAtmosphere(runtime);

		int nextIndex = EoH_RT_TraderRouteSystem.GetNextRouteIndex(profile, runtime);
		if (nextIndex < 0)
			return;

		//! Marker disappears for everyone as soon as trader moves
		ClearMarkerForAllPlayers(profile.TraderId);
		m_RevealedMarkers.Set(profile.TraderId, false);

		runtime.CurrentRouteIndex = nextIndex;

		EoH_RT_RouteNode node = EoH_RT_TraderRouteSystem.GetCurrentNode(profile, runtime);
		if (!node)
			return;

		PlaceTraderObject(runtime.TraderObject, node.Position, node.Orientation);

		runtime.LastMoveServerTime = GetGame().GetTime();

		EoH_RT_AIIntegration.SpawnEscort(profile, runtime, runtime.TraderObject.GetPosition());
		//! EoH_RT_Atmosphere.SpawnAtmosphere(profile, runtime, runtime.TraderObject.GetPosition());

		//! Do NOT broadcast marker here anymore
		BroadcastMoveNotification(profile, node);

		Print("[EoH_RT] Moved trader " + profile.TraderId + " to " + runtime.TraderObject.GetPosition().ToString());
	}

	//! Call this from whatever file/menu/action currently opens your trader interaction
	void RevealMarkerForObjectToAllPlayers(Object obj)
	{
		if (!GetGame().IsServer() || !obj)
			return;

		EoH_RT_TraderRuntime runtime = GetRuntimeByObject(obj);
		if (!runtime || !runtime.IsSpawned || !runtime.TraderObject)
			return;

		EoH_RT_TraderProfile profile = m_Config.FindProfile(runtime.TraderId);
		if (!profile)
			return;

		m_RevealedMarkers.Set(profile.TraderId, true);
		BroadcastMarker(profile, runtime.TraderObject.GetPosition());

		Print("[EoH_RT] Revealed marker for " + profile.TraderId + " to all players");
	}

	void SendAllMarkersToPlayer(PlayerBase player)
	{
		//! Only send markers that are currently globally revealed
		if (!GetGame().IsServer() || !player || !player.GetIdentity())
			return;

		foreach (string traderId, EoH_RT_TraderRuntime runtime : m_Runtimes)
		{
			if (!runtime || !runtime.IsSpawned || !runtime.TraderObject)
				continue;

			bool revealed = false;
			if (!m_RevealedMarkers.Find(traderId, revealed) || !revealed)
				continue;

			EoH_RT_TraderProfile profile = m_Config.FindProfile(traderId);
			if (!profile)
				continue;

			EoH_RT_MarkerData data = new EoH_RT_MarkerData(traderId, profile.MarkerLabel, runtime.TraderObject.GetPosition());
			GetGame().RPCSingleParam(player, EoH_RT_RPC.ADD_OR_UPDATE_MARKER, new Param1<ref EoH_RT_MarkerData>(data), true, player.GetIdentity());
		}
	}

	void BroadcastMarker(EoH_RT_TraderProfile profile, vector pos)
	{
		if (!m_Config || !m_Config.ShowMapMarkers || !profile)
			return;

		bool revealed = false;
		if (!m_RevealedMarkers.Find(profile.TraderId, revealed) || !revealed)
			return;

		array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);

		foreach (Man man : players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.GetIdentity())
				continue;

			EoH_RT_MarkerData data = new EoH_RT_MarkerData(profile.TraderId, profile.MarkerLabel, pos);
			GetGame().RPCSingleParam(player, EoH_RT_RPC.ADD_OR_UPDATE_MARKER, new Param1<ref EoH_RT_MarkerData>(data), true, player.GetIdentity());
		}
	}

	void ClearMarkerForAllPlayers(string traderId)
	{
		if (!GetGame().IsServer())
			return;

		array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);

		foreach (Man man : players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.GetIdentity())
				continue;

			GetGame().RPCSingleParam(player, EoH_RT_RPC.REMOVE_MARKER, new Param1<string>(traderId), true, player.GetIdentity());
		}
	}

	void BroadcastMoveNotification(EoH_RT_TraderProfile profile, EoH_RT_RouteNode node)
	{
		if (!m_Config || !m_Config.AnnounceMoves || !profile || !node)
			return;

		string msg = profile.AnnouncementTemplate;
		msg.Replace("%TRADER%", profile.DisplayName);
		msg.Replace("%LOCATION%", node.Name);

		array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);

		foreach (Man man : players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.GetIdentity())
				continue;

			EoH_RT_NotificationData notif = new EoH_RT_NotificationData(profile.DisplayName, msg, "popup");
			GetGame().RPCSingleParam(player, EoH_RT_RPC.NOTIFICATION, new Param1<ref EoH_RT_NotificationData>(notif), true, player.GetIdentity());
		}
	}
};