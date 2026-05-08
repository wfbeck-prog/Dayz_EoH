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
				EoH_CBD_MarkerHelper.SendToPlayer(player, room);
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
				Print("[EoH_CBD][WARN] Invalid door index " + room.LootRoomDoorIndex.ToString() + " for room " + room.LootRoomName + ".");
				continue;
			}

			if (building.IsDoorOpen(room.LootRoomDoorIndex))
				return true;
		}

		return false;
	}

	string GetMarkerId(LootSystemRoom room)
	{
		return EoH_CBD_MarkerHelper.GetMarkerId(room.LootRoomName);
	}
};