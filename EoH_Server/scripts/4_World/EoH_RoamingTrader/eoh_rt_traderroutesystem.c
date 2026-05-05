class EoH_RT_TraderRouteSystem
{
	static int GetNextRouteIndex(EoH_RT_TraderProfile profile, EoH_RT_TraderRuntime runtime)
	{
		if (!profile || !runtime || !profile.Route || profile.Route.Count() == 0)
			return -1;

		int nextIndex = runtime.CurrentRouteIndex + 1;
		if (nextIndex >= profile.Route.Count())
			nextIndex = 0;
		return nextIndex;
	}

	static EoH_RT_RouteNode GetCurrentNode(EoH_RT_TraderProfile profile, EoH_RT_TraderRuntime runtime)
	{
		if (!profile || !runtime || !profile.Route || profile.Route.Count() == 0)
			return NULL;
		if (runtime.CurrentRouteIndex < 0 || runtime.CurrentRouteIndex >= profile.Route.Count())
			return NULL;
		return profile.Route.Get(runtime.CurrentRouteIndex);
	}
};
