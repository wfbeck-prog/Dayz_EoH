class EoH_CBD_MarkerHelper
{
	static string GetMarkerId(string roomName)
	{
		return "KR_CBD_ROOM_" + roomName;
	}

	static int GetTierFromName(string roomName)
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

	static int GetColor(int tier)
	{
		switch (tier)
		{
			case 1: return ARGB(255, 40, 200, 40);
			case 2: return ARGB(255, 50, 120, 255);
			case 3: return ARGB(255, 220, 180, 40);
			case 4: return ARGB(255, 255, 120, 40);
			case 5: return ARGB(255, 220, 40, 40);
		}

		return ARGB(255, 255, 60, 60);
	}

	static string GetLabel(LootSystemRoom room)
	{
		int tier = GetTierFromName(room.LootRoomName);
		if (tier > 0)
			return "CBD Loot Room T" + tier.ToString() + " OPEN";

		return "CBD Loot Room OPEN";
	}

	static void Broadcast(LootSystemRoom room)
	{
		if (!room || !GetGame().IsServer())
			return;

		EoH_Notifications.SendToAll("CBD ROOM BREACHED", room.LootRoomName + " has been opened.");
		Print("[EoH_CBD] CBD room opened: " + room.LootRoomName + " at " + room.LootRoomPosition.ToString());
	}

	static void SendToPlayer(PlayerBase player, LootSystemRoom room)
	{
		if (!player || !player.GetIdentity() || !room)
			return;

		EoH_Notifications.SendToPlayer(player, "CBD ROOM BREACHED", GetLabel(room));
	}

	static void Clear(LootSystemRoom room)
	{
		if (!room || !GetGame().IsServer())
			return;

		ClearByName(room.LootRoomName);
	}

	static void ClearByName(string roomName)
	{
		string markerId = GetMarkerId(roomName);
		Print("[EoH_CBD] Cleared marker placeholder " + markerId);
	}
}
