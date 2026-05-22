class EoH_RT_TraderManager
{
	private static ref EoH_RT_TraderManager s_Instance;
	protected ref EoH_RT_GlobalConfig m_Config;
	protected ref map<string, ref EoH_RT_TraderRuntime> m_Runtimes;
	protected ref map<string, bool> m_RevealedMarkers;
	protected ref map<string, int> m_RelocationGraceStart;
	protected bool m_Initialized;
	static const float EOH_RT_RELOCATION_HOLD_RADIUS = 30.0;
	static const float EOH_RT_PROXIMITY_REVEAL_RADIUS = 35.0;
	static const int EOH_RT_RELOCATION_MAX_GRACE_MS = 900000;

	static EoH_RT_TraderManager Get()
	{
		if (!s_Instance)
			s_Instance = new EoH_RT_TraderManager();
		return s_Instance;
	}

	static EoH_RT_TraderManager GetInstance()
	{
		return Get();
	}

	void EoH_RT_TraderManager()
	{
		m_Config = NULL;
		m_Runtimes = new map<string, ref EoH_RT_TraderRuntime>();
		m_RevealedMarkers = new map<string, bool>();
		m_RelocationGraceStart = new map<string, int>();
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

			m_Runtimes.Set(profile.TraderId, runtime);
			m_RevealedMarkers.Set(profile.TraderId, false);
			m_RelocationGraceStart.Set(profile.TraderId, 0);
		}

		m_Initialized = true;
		Print("[EoH_RT] Initialized inside EoH_Server");
	}

	void Update()
	{
		if (!m_Initialized)
			Initialize();

		if (!m_Initialized || !m_Config || !m_Config.Enabled)
			return;

		foreach (string id, EoH_RT_TraderRuntime runtime : m_Runtimes)
		{
			EoH_RT_TraderProfile profile = m_Config.FindProfile(id);
			if (!profile)
				continue;

			EoH_RT_RouteNode currentNode = EoH_RT_TraderRouteSystem.GetCurrentNode(profile, runtime);
			bool revealed = false;
			m_RevealedMarkers.Find(profile.TraderId, revealed);

			if (m_Config.RequirePlayerNearby && currentNode && !revealed && !IsSurvivorNearPosition(currentNode.Position, m_Config.ActivationRadius))
			{
				if (runtime.IsSpawned)
					DeactivateTrader(profile, runtime);
				else if (m_Config.EnableDebug)
					Print("[EoH_RT][PROXIMITY] Trader inactive traderId=" + profile.TraderId + " no players within " + m_Config.ActivationRadius.ToString() + "m of route node " + currentNode.Name);

				continue;
			}

			if (!runtime.IsSpawned || !runtime.TraderObject)
			{
				SpawnTraderAtCurrentNode(runtime);
				continue;
			}

			CheckProximityReveal(profile, runtime);

			if (ShouldAdvanceRoute(profile, runtime))
				MoveTraderToNextNode(profile, runtime);
		}
	}

	bool IsSurvivorNearPosition(vector pos, float radius)
	{
		array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);

		foreach (Man man : players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.GetIdentity() || !player.IsAlive())
				continue;

			if (vector.Distance(player.GetPosition(), pos) <= radius)
				return true;
		}

		return false;
	}

	void DeactivateTrader(EoH_RT_TraderProfile profile, EoH_RT_TraderRuntime runtime)
	{
		if (!profile || !runtime)
			return;

		EoH_RT_AIIntegration.CleanupEscort(runtime);
		HideMarkerForAllPlayers(profile.TraderId);
		runtime.IsSpawned = false;
		m_RelocationGraceStart.Set(profile.TraderId, 0);
		Print("[EoH_RT][PROXIMITY] Deactivated trader " + profile.TraderId + " because no players are within " + m_Config.ActivationRadius.ToString() + "m.");
	}

	void CheckProximityReveal(EoH_RT_TraderProfile profile, EoH_RT_TraderRuntime runtime)
	{
		if (!profile || !runtime || !runtime.TraderObject)
			return;

		bool revealed = false;
		m_RevealedMarkers.Find(profile.TraderId, revealed);
		if (revealed)
			return;

		array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);

		foreach (Man man : players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.GetIdentity() || !player.IsAlive())
				continue;

			float dist = vector.Distance(player.GetPosition(), runtime.TraderObject.GetPosition());
			if (dist > EOH_RT_PROXIMITY_REVEAL_RADIUS)
				continue;

			RevealTraderGlobally(profile, runtime.TraderObject.GetPosition(), "PROXIMITY", player);
			return;
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

		int now = GetGame().GetTime();
		int elapsed = now - runtime.LastMoveServerTime;
		if (elapsed < stopMinutes * 60 * 1000)
			return false;

		if (IsSurvivorNearTrader(runtime, EOH_RT_RELOCATION_HOLD_RADIUS))
		{
			int graceStart = 0;
			m_RelocationGraceStart.Find(runtime.TraderId, graceStart);

			if (graceStart <= 0)
			{
				m_RelocationGraceStart.Set(runtime.TraderId, now);
				Print("[EoH_RT] Relocation delayed for " + runtime.TraderId + ": survivor within " + EOH_RT_RELOCATION_HOLD_RADIUS.ToString() + "m.");
				return false;
			}

			if (now - graceStart < EOH_RT_RELOCATION_MAX_GRACE_MS)
				return false;

			Print("[EoH_RT] Relocation grace expired for " + runtime.TraderId + ". Moving despite nearby survivor.");
		}

		m_RelocationGraceStart.Set(runtime.TraderId, 0);
		return true;
	}

	bool IsSurvivorNearTrader(EoH_RT_TraderRuntime runtime, float radius)
	{
		if (!runtime || !runtime.TraderObject)
			return false;

		return IsSurvivorNearPosition(runtime.TraderObject.GetPosition(), radius);
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

		bool revealed = false;
		m_RevealedMarkers.Find(profile.TraderId, revealed);
		if (m_Config.RequirePlayerNearby && !revealed && !IsSurvivorNearPosition(node.Position, m_Config.ActivationRadius))
		{
			if (m_Config.EnableDebug)
				Print("[EoH_RT][PROXIMITY] Spawn skipped traderId=" + profile.TraderId + " no players within " + m_Config.ActivationRadius.ToString() + "m of route node " + node.Name);
			return;
		}

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
		runtime.CurrentPosition = runtime.TraderObject.GetPosition();

		EoH_RT_AIIntegration.CleanupEscort(runtime);
		EoH_RT_AIIntegration.SpawnEscort(profile, runtime, runtime.TraderObject.GetPosition());

		if (!revealed)
			m_RevealedMarkers.Set(profile.TraderId, false);
		m_RelocationGraceStart.Set(profile.TraderId, 0);

		Print("[EoH_RT][PROXIMITY] Activated trader " + profile.TraderId + " at " + runtime.TraderObject.GetPosition().ToString());
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
		traderObj.SetOrientation(Vector(yaw, 0, 0));

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

		int nextIndex = EoH_RT_TraderRouteSystem.GetNextRouteIndex(profile, runtime);
		if (nextIndex < 0)
			return;

		HideMarkerForAllPlayers(profile.TraderId);

		runtime.CurrentRouteIndex = nextIndex;

		EoH_RT_RouteNode node = EoH_RT_TraderRouteSystem.GetCurrentNode(profile, runtime);
		if (!node)
			return;

		bool revealed = false;
		m_RevealedMarkers.Find(profile.TraderId, revealed);
		if (m_Config.RequirePlayerNearby && !revealed && !IsSurvivorNearPosition(node.Position, m_Config.ActivationRadius))
		{
			DeactivateTrader(profile, runtime);
			return;
		}

		PlaceTraderObject(runtime.TraderObject, node.Position, node.Orientation);

		runtime.LastMoveServerTime = GetGame().GetTime();
		runtime.CurrentPosition = runtime.TraderObject.GetPosition();
		m_RelocationGraceStart.Set(profile.TraderId, 0);

		EoH_RT_AIIntegration.SpawnEscort(profile, runtime, runtime.TraderObject.GetPosition());
		BroadcastMoveNotification(profile, node);

		Print("[EoH_RT] Moved trader " + profile.TraderId + " to " + runtime.TraderObject.GetPosition().ToString());
	}

	void RevealMarkerForObjectToAllPlayers(Object obj)
	{
		if (!GetGame().IsServer() || !obj)
			return;

		EoH_RT_TraderRuntime runtime = GetRuntimeByObject(obj);
		if (!runtime)
			return;

		Print("[EoH_RT] Ignored legacy object-only trader reveal traderId=" + runtime.TraderId + ". Awaiting player proximity or intel.");
	}

	bool RevealNearestHiddenTraderToPlayer(PlayerBase player)
	{
		if (!GetGame().IsServer() || !player || !player.GetIdentity())
			return false;

		if (!m_Initialized)
			Initialize();

		float bestDistance = 999999.0;
		EoH_RT_TraderProfile bestProfile = NULL;
		EoH_RT_TraderRuntime bestRuntime = NULL;
		vector bestPos = "0 0 0".ToVector();

		foreach (string traderId, EoH_RT_TraderRuntime runtime : m_Runtimes)
		{
			if (!runtime)
				continue;

			bool alreadyRevealed = false;
			m_RevealedMarkers.Find(traderId, alreadyRevealed);
			if (alreadyRevealed)
				continue;

			EoH_RT_TraderProfile profile = m_Config.FindProfile(traderId);
			if (!profile)
				continue;

			vector candidatePos = "0 0 0".ToVector();
			if (runtime.IsSpawned && runtime.TraderObject)
				candidatePos = runtime.TraderObject.GetPosition();
			else
			{
				EoH_RT_RouteNode node = EoH_RT_TraderRouteSystem.GetCurrentNode(profile, runtime);
				if (node)
					candidatePos = node.Position;
			}

			if (candidatePos == "0 0 0".ToVector())
				continue;

			float dist = vector.Distance(player.GetPosition(), candidatePos);
			if (dist < bestDistance)
			{
				bestDistance = dist;
				bestRuntime = runtime;
				bestProfile = profile;
				bestPos = candidatePos;
			}
		}

		if (!bestProfile || !bestRuntime || bestPos == "0 0 0".ToVector())
			return false;

		RevealTraderGlobally(bestProfile, bestPos, "INTEL", player);
		if (!bestRuntime.IsSpawned || !bestRuntime.TraderObject)
			SpawnTraderAtCurrentNode(bestRuntime);

		return true;
	}

	void RevealTraderGlobally(EoH_RT_TraderProfile profile, vector pos, string reason, PlayerBase player = null)
	{
		if (!profile)
			return;

		bool alreadyRevealed = false;
		m_RevealedMarkers.Find(profile.TraderId, alreadyRevealed);
		if (alreadyRevealed)
			return;

		m_RevealedMarkers.Set(profile.TraderId, true);
		BroadcastMarker(profile, pos);

		string playerName = "unknown survivor";
		if (player && player.GetIdentity())
			playerName = player.GetIdentity().GetName();

		if (reason == "PROXIMITY")
			EoH_Notifications.SendToAll("ROAMING TRADER FOUND", profile.DisplayName + " was found in the field. Survivors will converge on the location.");
		else
			EoH_Notifications.SendToAll("ROAMING TRADER SIGNAL", profile.DisplayName + " signal was decoded. Survivors will converge on the location.");

		Print("[EoH_RT] Trader revealed globally reason=" + reason + " traderId=" + profile.TraderId + " pos=" + pos.ToString() + " by=" + playerName);
	}

	void SendAllMarkersToPlayer(PlayerBase player)
	{
		if (!GetGame().IsServer() || !player || !player.GetIdentity())
			return;

		foreach (string traderId, EoH_RT_TraderRuntime runtime : m_Runtimes)
		{
			bool revealed = false;
			if (!m_RevealedMarkers.Find(traderId, revealed) || !revealed)
				continue;

			EoH_RT_TraderProfile profile = m_Config.FindProfile(traderId);
			if (!profile)
				continue;

			vector markerPos = "0 0 0".ToVector();
			if (runtime && runtime.IsSpawned && runtime.TraderObject)
				markerPos = runtime.TraderObject.GetPosition();
			else if (runtime)
			{
				EoH_RT_RouteNode node = EoH_RT_TraderRouteSystem.GetCurrentNode(profile, runtime);
				if (node)
					markerPos = node.Position;
			}

			if (markerPos == "0 0 0".ToVector())
				continue;

			EoH_MarkerData data = BuildTraderMarkerData(profile, markerPos);
			EoH_MarkerService.SendToPlayer(player, data);
		}
	}

	void BroadcastMarker(EoH_RT_TraderProfile profile, vector pos)
	{
		if (!m_Config || !m_Config.ShowMapMarkers || !profile)
			return;

		bool revealed = false;
		if (!m_RevealedMarkers.Find(profile.TraderId, revealed) || !revealed)
			return;

		EoH_MarkerService.Broadcast(BuildTraderMarkerData(profile, pos));
		Print("[EoH_RT] Broadcast trader marker traderId=" + profile.TraderId + " pos=" + pos.ToString());
	}

	EoH_MarkerData BuildTraderMarkerData(EoH_RT_TraderProfile profile, vector pos)
	{
		EoH_MarkerData data = new EoH_MarkerData(profile.TraderId, profile.MarkerLabel, pos);
		data.Category = EoH_MarkerCategory.TRADER;
		data.State = EoH_MarkerState.NORMAL;
		data.Icon = "Trader";
		data.Is3D = 1;
		data.Pulse = 0;
		data.Color = ARGB(255, 0, 200, 0);
		data.BaseColor = data.Color;
		data.Normalize();
		return data;
	}

	void HideMarkerForAllPlayers(string traderId)
	{
		if (!GetGame().IsServer() || traderId == "")
			return;

		m_RevealedMarkers.Set(traderId, false);
		ClearMarkerForAllPlayers(traderId);
		Print("[EoH_RT] Trader marker hidden and reveal state reset traderId=" + traderId);
	}

	void ClearMarkerForAllPlayers(string traderId)
	{
		if (!GetGame().IsServer() || traderId == "")
			return;

		array<string> markerIds = new array<string>();
		markerIds.Insert(traderId);
		markerIds.Insert("EoH_RT_" + traderId);
		markerIds.Insert("EoH_TRADER_" + traderId);
		markerIds.Insert("EoH_MARKER_" + traderId);

		foreach (string markerId : markerIds)
		{
			EoH_MarkerService.RemoveFromAll(markerId);
			Print("[EoH_RT] Requested marker removal id=" + markerId);
		}
	}

	void BroadcastMoveNotification(EoH_RT_TraderProfile profile, EoH_RT_RouteNode node)
	{
		if (!m_Config || !m_Config.AnnounceMoves || !profile || !node)
			return;

		string msg = profile.AnnouncementTemplate;
		msg.Replace("%TRADER%", profile.DisplayName);
		msg.Replace("%LOCATION%", node.Name);

		EoH_Notifications.SendToAll("ROAMING TRADER MOVED", msg);
		Print("[EoH_RT] Move notification sent: " + msg);
	}
};
