class EoH_RT_Atmosphere
{
	static void SpawnAtmosphere(EoH_RT_TraderProfile profile, EoH_RT_TraderRuntime runtime, vector pos)
	{
		if (!GetGame().IsServer() || !profile || !runtime)
			return;

		int _noop = 0;
	}

	static void CleanupAtmosphere(EoH_RT_TraderRuntime runtime)
	{
		if (!runtime)
			return;

		int _noop = 0;
	}
}