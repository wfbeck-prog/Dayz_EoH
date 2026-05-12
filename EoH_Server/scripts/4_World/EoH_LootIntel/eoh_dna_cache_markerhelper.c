class EoH_DNACacheTier
{
	static const string YELLOW = "Yellow";
	static const string GREEN = "Green";
	static const string BLUE = "Blue";
	static const string RED = "Red";
}

class EoH_DNACacheMarkerHelper
{
	static void BroadcastCacheOpened(string tier, vector position)
	{
		if (!GetGame().IsServer())
			return;

		if (tier == string.Empty || tier == "Purple")
			return;

		EoH_MarkerData data = BuildCacheOpenedMarker(tier, position);
		EoH_MarkerService.Broadcast(data);

		string title = GetOpenedTitle(tier);
		string body = GetOpenedBody(tier);
		EoH_Notifications.SendToAll(title, body);

		Print("[EoH_DNAIntel] Cache opened marker broadcast tier=" + tier + " pos=" + position.ToString());
	}

	static void RemoveCacheOpenedMarker(string tier, vector position)
	{
		if (!GetGame().IsServer())
			return;

		EoH_MarkerService.RemoveFromAll(BuildMarkerId(tier, position));
	}

	static void BroadcastCacheOpenedTimed(string tier, vector position, int lifetimeMs = 900000)
	{
		BroadcastCacheOpened(tier, position);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RemoveCacheOpenedMarker, lifetimeMs, false, tier, position);
	}

	static EoH_MarkerData BuildCacheOpenedMarker(string tier, vector position)
	{
		EoH_MarkerData data = new EoH_MarkerData(BuildMarkerId(tier, position), GetMarkerLabel(tier), position);
		data.Category = EoH_MarkerCategory.CACHE;
		data.State = EoH_MarkerState.ACTIVE;
		data.Icon = "Questionmark";
		data.Is3D = 1;
		data.Pulse = 1;
		data.Color = GetTierColor(tier);
		data.BaseColor = data.Color;
		data.Normalize();
		return data;
	}

	static string BuildMarkerId(string tier, vector position)
	{
		return "EoH_DNA_CACHE_OPENED_" + tier + "_" + Math.Round(position[0]).ToString() + "_" + Math.Round(position[2]).ToString();
	}

	static string GetMarkerLabel(string tier)
	{
		if (tier == EoH_DNACacheTier.RED)
			return "T4 Cache Opened";

		if (tier == EoH_DNACacheTier.BLUE)
			return "T3 Cache Opened";

		if (tier == EoH_DNACacheTier.GREEN)
			return "T2 Cache Opened";

		return "T1 Cache Opened";
	}

	static string GetOpenedTitle(string tier)
	{
		if (tier == EoH_DNACacheTier.RED)
			return "T4 CACHE BREACHED";

		if (tier == EoH_DNACacheTier.BLUE)
			return "T3 CACHE BREACHED";

		if (tier == EoH_DNACacheTier.GREEN)
			return "T2 CACHE BREACHED";

		return "T1 CACHE BREACHED";
	}

	static string GetOpenedBody(string tier)
	{
		if (tier == EoH_DNACacheTier.RED)
			return "A high-value cache signal has activated. Expect survivor movement toward the zone.";

		if (tier == EoH_DNACacheTier.BLUE)
			return "A military-grade cache signal has activated.";

		if (tier == EoH_DNACacheTier.GREEN)
			return "A field cache signal has activated.";

		return "A low-tier cache signal has activated.";
	}

	static int GetTierColor(string tier)
	{
		if (tier == EoH_DNACacheTier.RED)
			return ARGB(255, 220, 40, 40);

		if (tier == EoH_DNACacheTier.BLUE)
			return ARGB(255, 60, 120, 255);

		if (tier == EoH_DNACacheTier.GREEN)
			return ARGB(255, 60, 220, 80);

		return ARGB(255, 255, 220, 60);
	}
};
