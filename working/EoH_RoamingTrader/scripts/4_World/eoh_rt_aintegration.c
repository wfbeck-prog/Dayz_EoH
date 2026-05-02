class EoH_RT_AIIntegration
{
	static void SpawnEscort(EoH_RT_TraderProfile profile, EoH_RT_TraderRuntime runtime, vector centerPos)
	{
		if (!GetGame().IsServer() || !profile || !runtime || !profile.Escort || !profile.Escort.Enabled)
			return;

		CleanupEscort(runtime);

		if (!runtime.EscortUnits)
			runtime.EscortUnits = new array<Object>();

		int count = Math.RandomIntInclusive(profile.Escort.MinGuards, profile.Escort.MaxGuards);
		if (count <= 0)
			return;

		eAIFaction faction = GetEscortFaction(profile);
		eAIGroup group = eAIGroup.CreateGroup(faction);

		float ringRadius = Math.Max(3.0, profile.Escort.GuardRadius * 0.75);

		for (int i = 0; i < count; i++)
		{
			float angleDeg = (360.0 / count) * i;
			float angleRad = angleDeg * Math.DEG2RAD;

			float offsetX = Math.Cos(angleRad) * ringRadius;
			float offsetZ = Math.Sin(angleRad) * ringRadius;

			vector pos = centerPos + Vector(offsetX, 0, offsetZ);
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.05;

			//! Face outward from the trader
			vector dir = pos - centerPos;
			float yaw = Math.Atan2(dir[0], dir[2]) * Math.RAD2DEG;

			Object guardObj = SpawnEscortUnit(profile, pos, yaw, group);
			if (guardObj)
				runtime.EscortUnits.Insert(guardObj);
		}
	}

	static Object SpawnEscortUnit(EoH_RT_TraderProfile profile, vector pos, float yaw, eAIGroup group)
	{
		Object obj = GetGame().CreateObject("eAI_SurvivorM_Mirek", pos, false, true);
		if (!obj)
		{
			Print("[EoH_RT][ERROR] Failed to spawn escort AI");
			return NULL;
		}

		eAIBase ai = eAIBase.Cast(obj);
		if (!ai)
		{
			Print("[EoH_RT][ERROR] Spawned escort is not eAIBase");
			GetGame().ObjectDelete(obj);
			return NULL;
		}

		ai.SetPosition(pos);
		ai.SetOrientation(Vector(yaw, 0, 0));

		if (group)
			ai.SetGroup(group);

		ApplyEscortSetup(ai, profile, yaw);

		Print("[EoH_RT] Spawned escort AI with loadout " + GetEscortLoadout(profile) + " for " + profile.TraderId);
		return ai;
	}

	static void ApplyEscortSetup(eAIBase ai, EoH_RT_TraderProfile profile, float yaw)
	{
		if (!ai || !profile)
			return;

		string loadout = GetEscortLoadout(profile);

		ExpansionHumanLoadout.Apply(ai, loadout, true);

		ai.SetOrientation(Vector(yaw, 0, 0));
		ai.eAI_SetLootingBehavior(eAILootingBehavior.NONE);
		ai.SetMovementSpeedLimits(1, 3);

		//! Force primary into hands after inventory settles
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ForcePrimaryToHands, 1000, false, ai);
	}

	static void ForcePrimaryToHands(eAIBase ai)
	{
		if (!ai || !ai.IsAlive())
			return;

		EntityAI shoulder = ai.FindAttachmentBySlotName("Shoulder");
		if (!shoulder)
		{
			Print("[EoH_RT][WARN] No shoulder weapon found for AI " + ai);
			return;
		}

		InventoryLocation src = new InventoryLocation();
		InventoryLocation dst = new InventoryLocation();

		if (!shoulder.GetInventory().GetCurrentInventoryLocation(src))
			return;

		if (!ai.GetInventory().FindFreeLocationFor(shoulder, FindInventoryLocationType.HANDS, dst))
			return;

		GameInventory.LocationSyncMoveEntity(src, dst);
	}

	static string GetEscortLoadout(EoH_RT_TraderProfile profile)
	{
		if (!profile)
			return "EoH_BlackMarketEscort";

		if (profile.TraderId == "eoh_drug_trader")
			return "EoH_DrugEscort";

		if (profile.TraderId == "eoh_black_market")
			return "EoH_BlackMarketEscort";

		return "EoH_BlackMarketEscort";
	}

	static eAIFaction GetEscortFaction(EoH_RT_TraderProfile profile)
	{
		return new eAIFactionGuards();
	}

	static void CleanupEscort(EoH_RT_TraderRuntime runtime)
	{
		if (!runtime || !runtime.EscortUnits)
			return;

		for (int i = runtime.EscortUnits.Count() - 1; i >= 0; i--)
		{
			Object unit = runtime.EscortUnits.Get(i);
			if (unit)
				GetGame().ObjectDelete(unit);
		}

		runtime.EscortUnits.Clear();
	}
}