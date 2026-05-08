class EoH_CBD_Observer
{
	private static ref EoH_CBD_Observer s_Instance;
	protected ref map<string, bool> m_LastState;

	static EoH_CBD_Observer GetInstance()
	{
		if (!s_Instance)
			s_Instance = new EoH_CBD_Observer();

		return s_Instance;
	}

	void EoH_CBD_Observer()
	{
		m_LastState = new map<string, bool>();
	}

	void Update()
	{
		if (!GetGame().IsServer())
			return;

		LootSystemEntryModule module = LootSystemEntryModule.Cast(CF_ModuleCoreManager.Get(LootSystemEntryModule));
		if (!module || !module.settings || !module.settings.LootRooms)
			return;

		foreach (LootSystemRoom room : module.settings.LootRooms)
		{
			if (!room)
				continue;

			string id = GetMarkerId(room);
			bool isOpen = IsDoorOpen(room);
			bool wasOpen = false;

			if (!m_LastState.Find(id, wasOpen))
				wasOpen = false;

			if (isOpen && !wasOpen)
				BroadcastMarker(room);

			if (!isOpen && wasOpen)
				ClearMarker(id);

			m_LastState.Set(id, isOpen);
		}
	}

	void SendCurrentMarkersToPlayer(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return;

		LootSystemEntryModule module = LootSystemEntryModule.Cast(CF_ModuleCoreManager.Get(LootSystemEntryModule));
		if (!module || !module.settings || !module.settings.LootRooms)
			return;

		foreach (LootSystemRoom room : module.settings.LootRooms)
		{
			if (!room)
				continue;

			if (IsDoorOpen(room))
				SendMarkerToPlayer(player, room);
		}
	}

bool IsDoorOpen(LootSystemRoom room)
{
	if (!room)
		return false;

	if (room.LootRoomDoorIndex < 0)
		return false;

	array<Object> objects = new array<Object>();
	array<CargoBase> cargos = new array<CargoBase>();

	GetGame().GetObjectsAtPosition(room.LootRoomPosition, 2.5, objects, cargos);

	foreach (Object obj : objects)
	{
		if (!obj)
			continue;

		Building building = Building.Cast(obj);
		if (!building)
			continue;

		int doorCount = building.GetDoorCount();

		if (doorCount <= 0)
			continue;

		if (room.LootRoomDoorIndex >= doorCount)
		{
			Print("[EoH_CBD][WARN] Invalid door index " + room.LootRoomDoorIndex.ToString() + " for room " + room.LootRoomName + " at " + room.LootRoomPosition.ToString() + ". Building has " + doorCount.ToString() + " doors.");
			continue;
		}

		if (building.IsDoorOpen(room.LootRoomDoorIndex))
			return true;
	}

	return false;
}

	string GetMarkerId(LootSystemRoom room)
	{
		return "CBD_ROOM_" + room.LootRoomName;
	}

	int GetTierFromName(string roomName)
	{
		string upper = roomName;
		upper.ToUpper();

		if (upper.IndexOf("T1_") == 0 || upper.IndexOf("TIER1") != -1)
			return 1;

		if (upper.IndexOf("T2_") == 0 || upper.IndexOf("TIER2") != -1)
			return 2;

		if (upper.IndexOf("T3_") == 0 || upper.IndexOf("TIER3") != -1)
			return 3;

		if (upper.IndexOf("T4_") == 0 || upper.IndexOf("TIER4") != -1)
			return 4;

		if (upper.IndexOf("T5_") == 0 || upper.IndexOf("TIER5") != -1)
			return 5;

		return 0;
	}

	int GetTierColor(int tier)
	{
		switch (tier)
		{
			case 1:
				return ARGB(255, 40, 200, 40);

			case 2:
				return ARGB(255, 50, 120, 255);

			case 3:
				return ARGB(255, 220, 180, 40);

			case 4:
				return ARGB(255, 255, 120, 40);

			case 5:
				return ARGB(255, 220, 40, 40);
		}

		return ARGB(255, 255, 50, 50);
	}

	string GetLabel(LootSystemRoom room)
	{
		int tier = GetTierFromName(room.LootRoomName);

		if (tier > 0)
			return "CBD Loot Room T" + tier.ToString();

		return "CBD Loot Room";
	}

void SendMarkerToPlayer(PlayerBase player, LootSystemRoom room)
{
	if (!player || !player.GetIdentity() || !room)
		return;

	int tier = GetTierFromName(room.LootRoomName);

	EoH_RT_MarkerData data = new EoH_RT_MarkerData();

	data.TraderId = "KR_" + GetMarkerId(room); // IMPORTANT PREFIX
data.Label = GetLabel(room);
data.Position = room.LootRoomPosition;

	data.Color = GetTierColor(tier);
	data.Pulse = 1;
	data.Icon = "Danger";
	data.Is3D = 1;

	Param1<ref EoH_RT_MarkerData> markerParam;
	markerParam = new Param1<ref EoH_RT_MarkerData>(data);

	GetGame().RPCSingleParam(player, EoH_RT_RPC.ADD_OR_UPDATE_KEYROOM_MARKER, markerParam, true, player.GetIdentity());
}

	void BroadcastMarker(LootSystemRoom room)
	{
		array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);

		foreach (Man man : players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.GetIdentity())
				continue;

			SendMarkerToPlayer(player, room);
		}
	}

void ClearMarker(string id)
{
	array<Man> players = new array<Man>();
	GetGame().GetPlayers(players);

	foreach (Man man : players)
	{
		PlayerBase player = PlayerBase.Cast(man);
		if (!player || !player.GetIdentity())
			continue;

		Param1<string> removeParam = new Param1<string>(id);

		GetGame().RPCSingleParam(player, EoH_RT_RPC.REMOVE_KEYROOM_MARKER, removeParam, true, player.GetIdentity());
	}
}
};