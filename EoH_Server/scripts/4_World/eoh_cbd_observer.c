class EoH_CBD_Observer
{
	private static ref EoH_CBD_Observer s_Instance;
	protected ref map<string, bool> m_LastState;
	protected ref map<string, int> m_LastDebug;
	protected bool m_StatePrimed;
	static const float EOH_CBD_DOOR_SCAN_RADIUS = 25.0;

	static EoH_CBD_Observer GetInstance()
	{
		if (!s_Instance)
			s_Instance = new EoH_CBD_Observer();

		return s_Instance;
	}

	void EoH_CBD_Observer()
	{
		m_LastState = new map<string, bool>();
		m_LastDebug = new map<string, int>();
		m_StatePrimed = false;
	}

	LootSystemEntryModule GetCBDModule()
	{
		return LootSystemEntryModule.Cast(CF_ModuleCoreManager.Get(LootSystemEntryModule));
	}

	LootSystemRoom FindNearestRoom(vector playerPos)
	{
		LootSystemEntryModule module = GetCBDModule();
		if (!module || !module.settings || !module.settings.LootRooms)
			return null;

		LootSystemRoom nearest;
		float nearestDist = 999999;

		foreach (LootSystemRoom room : module.settings.LootRooms)
		{
			if (!room)
				continue;

			float dist = vector.Distance(playerPos, room.LootRoomPosition);
			if (dist < nearestDist)
			{
				nearest = room;
				nearestDist = dist;
			}
		}

		return nearest;
	}

	void Update()
	{
		if (!GetGame().IsServer())
			return;

		LootSystemEntryModule module = GetCBDModule();
		if (!module || !module.settings || !module.settings.LootRooms)
		{
			DebugThrottled("CBD_MODULE", "[EoH_CBD][DEBUG] CBD module/settings/LootRooms unavailable.");
			return;
		}

		foreach (LootSystemRoom room : module.settings.LootRooms)
		{
			if (!room)
				continue;

			string id = GetMarkerId(room);
			bool isOpen = IsDoorOpen(room);
			bool wasOpen = false;
			bool known = m_LastState.Find(id, wasOpen);

			if (!m_StatePrimed || !known)
			{
				m_LastState.Set(id, isOpen);
				continue;
			}

			if (isOpen && !wasOpen)
			{
				Print("[EoH_CBD] Door opened: " + room.LootRoomName);
				EoH_CBD_MarkerHelper.Broadcast(room);
			}

			if (!isOpen && wasOpen)
			{
				Print("[EoH_CBD] Door closed: " + room.LootRoomName);
				EoH_CBD_MarkerHelper.Clear(room);
			}

			m_LastState.Set(id, isOpen);
		}

		if (!m_StatePrimed)
		{
			m_StatePrimed = true;
			Print("[EoH_CBD] Observer state primed. Startup room states recorded without broadcasting markers.");
		}
	}

	void SendCurrentMarkersToPlayer(PlayerBase player)
	{
		// Do not resend all currently-open CBD states on player connect.
		// Loot room intel and true open transitions handle marker visibility.
	}

	bool IsDoorOpen(LootSystemRoom room)
	{
		if (!room)
			return false;

		if (room.LootRoomDoorIndex < 0)
		{
			DebugThrottled(room.LootRoomName + "_NO_INDEX", "[EoH_CBD][DEBUG] Room has invalid door index: " + room.LootRoomName);
			return false;
		}

		array<Object> objects = new array<Object>();
		array<CargoBase> cargos = new array<CargoBase>();

		GetGame().GetObjectsAtPosition(room.LootRoomPosition, EOH_CBD_DOOR_SCAN_RADIUS, objects, cargos);

		bool foundBuilding = false;
		foreach (Object obj : objects)
		{
			if (!obj)
				continue;

			Building building = Building.Cast(obj);
			if (!building)
				continue;

			foundBuilding = true;
			int doorCount = building.GetDoorCount();

			if (doorCount <= 0)
				continue;

			if (room.LootRoomDoorIndex >= doorCount)
			{
				Print("[EoH_CBD][WARN] Invalid door index " + room.LootRoomDoorIndex.ToString() + " for room " + room.LootRoomName + ". Building=" + building.GetType() + " doorCount=" + doorCount.ToString());
				continue;
			}

			bool open = building.IsDoorOpen(room.LootRoomDoorIndex);
			DebugThrottled(room.LootRoomName + "_STATE", "[EoH_CBD][DEBUG] Room=" + room.LootRoomName + " building=" + building.GetType() + " doorIndex=" + room.LootRoomDoorIndex.ToString() + " open=" + open.ToString() + " roomPos=" + room.LootRoomPosition.ToString());

			if (open)
				return true;
		}

		if (!foundBuilding)
			DebugThrottled(room.LootRoomName + "_NO_BUILDING", "[EoH_CBD][DEBUG] No building found within " + EOH_CBD_DOOR_SCAN_RADIUS.ToString() + "m for room=" + room.LootRoomName + " pos=" + room.LootRoomPosition.ToString());

		return false;
	}

	void DebugThrottled(string key, string message)
	{
		int now = GetGame().GetTime();
		int last = 0;

		if (m_LastDebug && m_LastDebug.Find(key, last))
		{
			if (now - last < 30000)
				return;
		}

		if (m_LastDebug)
			m_LastDebug.Set(key, now);

		Print(message);
	}

	string GetMarkerId(LootSystemRoom room)
	{
		return EoH_CBD_MarkerHelper.GetMarkerId(room.LootRoomName);
	}
};