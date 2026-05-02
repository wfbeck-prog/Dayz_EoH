modded class PlayerBase
{
	private float m_EoH_RT_NextRevealCheckTime;

	override void OnPlayerLoaded()
	{
		super.OnPlayerLoaded();
		m_EoH_RT_NextRevealCheckTime = 0;
	}

	override void EOnFrame(IEntity other, float timeSlice)
	{
		super.EOnFrame(other, timeSlice);

		if (!GetGame().IsServer())
			return;

		float now = GetGame().GetTime();
		if (now < m_EoH_RT_NextRevealCheckTime)
			return;

		m_EoH_RT_NextRevealCheckTime = now + 1000;
		EoH_RT_CheckTraderRevealProximity();
	}

	void EoH_RT_CheckTraderRevealProximity()
	{
		EoH_RT_TraderManager manager = EoH_RT_TraderManager.GetInstance();
		if (!manager)
			return;

		array<Object> objects = new array<Object>();
		array<CargoBase> proxy = new array<CargoBase>();

		GetGame().GetObjectsAtPosition3D(GetPosition(), 3.0, objects, proxy);

		for (int i = 0; i < objects.Count(); i++)
		{
			Object obj = objects.Get(i);
			if (!obj)
				continue;

			EoH_RT_TraderRuntime runtime = manager.GetRuntimeByObject(obj);
			if (!runtime)
				continue;

			manager.RevealMarkerForObjectToAllPlayers(obj);
			return;
		}
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (GetGame().IsServer())
			return;

		if (rpc_type == EoH_RT_RPC.ADD_OR_UPDATE_KEYROOM_MARKER)
		{
			Param1<ref EoH_RT_MarkerData> keyRoomParam;

			if (!ctx.Read(keyRoomParam) || !keyRoomParam || !keyRoomParam.param1)
				return;

			if (keyRoomParam.param1.TraderId.IndexOf("KR_") != 0)
				return;

			EoH_RT_MarkerManager.GetInstance().SetMarker(keyRoomParam.param1);
			return;
		}

		if (rpc_type == EoH_RT_RPC.REMOVE_KEYROOM_MARKER)
		{
			Param1<string> removeKeyRoomParam;

			if (!ctx.Read(removeKeyRoomParam) || !removeKeyRoomParam)
				return;

			if (removeKeyRoomParam.param1.IndexOf("KR_") != 0)
				return;

			EoH_RT_MarkerManager.GetInstance().RemoveMarker(removeKeyRoomParam.param1);
			return;
		}

		if (rpc_type == EoH_RT_RPC.ADD_OR_UPDATE_MARKER)
		{
			Param1<ref EoH_RT_MarkerData> traderParam;

			if (!ctx.Read(traderParam) || !traderParam || !traderParam.param1)
				return;

			if (traderParam.param1.TraderId.IndexOf("KR_") == 0)
				return;

			EoH_RT_MarkerManager.GetInstance().SetMarker(traderParam.param1);
			return;
		}

		if (rpc_type == EoH_RT_RPC.REMOVE_MARKER)
		{
			Param1<string> removeTraderParam;

			if (!ctx.Read(removeTraderParam) || !removeTraderParam)
				return;

			if (removeTraderParam.param1.IndexOf("KR_") == 0)
				return;

			EoH_RT_MarkerManager.GetInstance().RemoveMarker(removeTraderParam.param1);
			return;
		}

		if (rpc_type == EoH_RT_RPC.NOTIFICATION)
		{
			Param1<ref EoH_RT_NotificationData> notificationParam;

			if (!ctx.Read(notificationParam) || !notificationParam || !notificationParam.param1)
				return;

			EoH_RT_ClientState.GetInstance().QueueNotification(notificationParam.param1);
			return;
		}
	}
}