class EoH_CBD_MarkerHelper
{
	static string GetMarkerId(string roomName)
	{
		return "CBD_" + roomName;
	}

	static int GetColor()
	{
		return ARGB(255, 255, 60, 60); // red
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

			EoH_RT_MarkerData data = new EoH_RT_MarkerData();

			data.TraderId = "KR_" + GetMarkerId(room.LootRoomName);
data.Label = room.LootRoomName;
data.Position = room.LootRoomPosition;

			data.Color = GetColor();
			data.Pulse = 1;
			data.Icon = "Danger";
			data.Is3D = 1;

			Param1<ref EoH_RT_MarkerData> markerParam;
			markerParam = new Param1<ref EoH_RT_MarkerData>(data);

			GetGame().RPCSingleParam(player, EoH_RT_RPC.ADD_OR_UPDATE_KEYROOM_MARKER, markerParam, true, player.GetIdentity());
		}
	}

	static void Clear(LootSystemRoom room)
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

			Param1<string> removeParam;
			removeParam = new Param1<string>(GetMarkerId(room.LootRoomName));

			GetGame().RPCSingleParam(player, EoH_RT_RPC.REMOVE_KEYROOM_MARKER, removeParam, true, player.GetIdentity());
		}
	}
}