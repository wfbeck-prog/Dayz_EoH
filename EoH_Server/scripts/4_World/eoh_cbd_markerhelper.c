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

		array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);

		foreach (Man man : players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.GetIdentity())
				continue;

			SendToPlayer(player, room);
		}

		EoH_Notifications.SendToAll("CBD ROOM BREACHED", room.LootRoomName + " has been opened.");
		Print("[EoH_CBD] Broadcast flash marker for room " + room.LootRoomName);
	}

	static void SendToPlayer(PlayerBase player, LootSystemRoom room)
	{
		if (!player || !player.GetIdentity() || !room)
			return;

		int tier = GetTierFromName(room.LootRoomName);

		EoH_RT_MarkerData data = new EoH_RT_MarkerData();
		data.TraderId = GetMarkerId(room.LootRoomName);
		data.Label = GetLabel(room);
		data.Position = room.LootRoomPosition;
		data.Color = GetColor(tier);
		data.Pulse = 1;
		data.Icon = "Danger";
		data.Is3D = 1;

		Param1<ref EoH_RT_MarkerData> markerParam = new Param1<ref EoH_RT_MarkerData>(data);
		GetGame().RPCSingleParam(player, EoH_RT_RPC.ADD_OR_UPDATE_KEYROOM_MARKER, markerParam, true, player.GetIdentity());
	}

	static void Clear(LootSystemRoom room)
	{
		if (!room || !GetGame().IsServer())
			return;

		ClearByName(room.LootRoomName);
	}

	static void ClearByName(string roomName)
	{
		array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);

		string markerId = GetMarkerId(roomName);

		foreach (Man man : players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.GetIdentity())
				continue;

			Param1<string> removeParam = new Param1<string>(markerId);
			GetGame().RPCSingleParam(player, EoH_RT_RPC.REMOVE_KEYROOM_MARKER, removeParam, true, player.GetIdentity());
		}

		Print("[EoH_CBD] Cleared marker " + markerId);
	}
}