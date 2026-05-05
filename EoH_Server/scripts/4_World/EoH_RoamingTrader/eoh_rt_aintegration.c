class EoH_RT_AIIntegration
{
	static void SpawnEscort(EoH_RT_TraderProfile profile, EoH_RT_TraderRuntime runtime, vector centerPos)
	{
		if (!GetGame().IsServer() || !profile || !runtime || !profile.Escort || !profile.Escort.Enabled)
			return;

		CleanupEscort(runtime);

		int count = Math.RandomIntInclusive(profile.Escort.MinGuards, profile.Escort.MaxGuards);
		if (count <= 0)
			return;

		eAIGroup group = eAIGroup.CreateGroup(new eAIFactionGuards());

		for (int i = 0; i < count; i++)
		{
			vector pos = centerPos + Vector(Math.RandomFloat(-6,6),0,Math.RandomFloat(-6,6));
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.05;

			Object obj = GetGame().CreateObject("eAI_SurvivorM_Mirek", pos, false, true);
			if (!obj)
				continue;

			eAIBase ai = eAIBase.Cast(obj);
			if (!ai)
				continue;

			ai.SetGroup(group);
			runtime.EscortUnits.Insert(obj);
		}
	}

	static void CleanupEscort(EoH_RT_TraderRuntime runtime)
	{
		if (!runtime || !runtime.EscortUnits)
			return;

		foreach (Object obj : runtime.EscortUnits)
		{
			if (obj)
				GetGame().ObjectDelete(obj);
		}

		runtime.EscortUnits.Clear();
	}
};
