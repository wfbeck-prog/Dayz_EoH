class EoH_DNACacheOpenBridge
{
	protected static ref map<string, int> s_OpenedCacheAlerts;
	static const int EOH_DNA_CACHE_ALERT_COOLDOWN_MS = 900000;

	static void EnsureInit()
	{
		if (!s_OpenedCacheAlerts)
			s_OpenedCacheAlerts = new map<string, int>();
	}

	static void OnCrateOpened(Object crate)
	{
		if (!GetGame().IsServer() || !crate)
			return;

		string type = crate.GetType();
		Print("[EoH_DNAHook] Bridge received crate type=" + type);

		string tier = GetTierFromCrate(crate);
		Print("[EoH_DNAHook] Tier detected=" + tier);

		if (tier == string.Empty || tier == "Purple")
		{
			Print("[EoH_DNAHook] Tier ignored or unresolved for crate=" + type);
			return;
		}

		vector pos = crate.GetPosition();
		string alertId = BuildAlertId(tier, pos);

		if (WasRecentlyAlerted(alertId))
		{
			Print("[EoH_DNAHook] Alert cooldown active for=" + alertId);
			return;
		}

		SetRecentlyAlerted(alertId);
		Print("[EoH_DNAHook] Broadcasting cache marker tier=" + tier + " pos=" + pos.ToString());
		EoH_DNACacheMarkerHelper.BroadcastCacheOpenedTimed(tier, pos, EOH_DNA_CACHE_ALERT_COOLDOWN_MS);
	}

	static void OnCrateOpenedAt(string tier, vector pos)
	{
		if (!GetGame().IsServer())
			return;

		if (tier == string.Empty || tier == "Purple")
			return;

		string alertId = BuildAlertId(tier, pos);
		if (WasRecentlyAlerted(alertId))
			return;

		SetRecentlyAlerted(alertId);
		EoH_DNACacheMarkerHelper.BroadcastCacheOpenedTimed(tier, pos, EOH_DNA_CACHE_ALERT_COOLDOWN_MS);
	}

	static string GetTierFromCrate(Object crate)
	{
		if (!crate)
			return string.Empty;

		string type = crate.GetType();

		if (type.Contains("Purple"))
			return "Purple";

		if (type.Contains("Red"))
			return EoH_DNACacheTier.RED;

		if (type.Contains("Blue"))
			return EoH_DNACacheTier.BLUE;

		if (type.Contains("Green"))
			return EoH_DNACacheTier.GREEN;

		if (type.Contains("Yellow"))
			return EoH_DNACacheTier.YELLOW;

		return string.Empty;
	}

	protected static bool WasRecentlyAlerted(string alertId)
	{
		EnsureInit();

		int lastAlert = 0;
		if (!s_OpenedCacheAlerts.Find(alertId, lastAlert))
			return false;

		return (GetGame().GetTime() - lastAlert) < EOH_DNA_CACHE_ALERT_COOLDOWN_MS;
	}

	protected static void SetRecentlyAlerted(string alertId)
	{
		EnsureInit();
		s_OpenedCacheAlerts.Set(alertId, GetGame().GetTime());
	}

	protected static string BuildAlertId(string tier, vector pos)
	{
		return tier + "_" + Math.Round(pos[0]).ToString() + "_" + Math.Round(pos[2]).ToString();
	}
};
