class EoH_RT_TraderRouteSystem
{
	static int GetNextRouteIndex(EoH_RT_TraderProfile profile, EoH_RT_TraderRuntime runtime)
	{
		if (!profile || !runtime || !profile.Route || profile.Route.Count() == 0)
			return -1;

		int count = profile.Route.Count();
		if (count == 1)
			return 0;

		int current = runtime.CurrentRouteIndex;
		int nextIndex = current;

		// EoH behavior: roaming traders should feel unpredictable.
		// Pick a random route node and avoid immediately reusing the current one.
		for (int attempts = 0; attempts < 10; attempts++)
		{
			nextIndex = Math.RandomInt(0, count);
			if (nextIndex != current)
				break;
		}

		// Failsafe: if random somehow returned current every time, move forward once.
		if (nextIndex == current)
		{
			nextIndex = current + 1;
			if (nextIndex >= count)
				nextIndex = 0;
		}

		Print("[EoH_RT] Random route selected traderId=" + runtime.TraderId + " current=" + current.ToString() + " next=" + nextIndex.ToString() + " routeCount=" + count.ToString());
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
