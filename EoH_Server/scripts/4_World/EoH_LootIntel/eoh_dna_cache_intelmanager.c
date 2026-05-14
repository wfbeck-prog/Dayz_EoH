class EoH_DNACacheIntelTarget
{
    string Tier;
    vector Position;
    string Label;

    void EoH_DNACacheIntelTarget(string tier = "", vector pos = "0 0 0", string label = "")
    {
        Tier = tier;
        Position = pos;
        Label = label;
    }
};

class EoH_DNACacheIntelManager
{
    protected static ref EoH_DNACacheIntelManager s_Instance;

    static EoH_DNACacheIntelManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_DNACacheIntelManager();
        return s_Instance;
    }

    bool RevealNearestCacheToPlayer(PlayerBase player)
    {
        if (!GetGame().IsServer() || !player || !player.GetIdentity())
            return false;

        EoH_DNACacheIntelTarget best = EoH_DNACacheOpenBridge.GetNearestRegisteredCache(player.GetPosition());
        if (!best)
        {
            Print("[EoH_DNACacheIntel] No registered LIVE DNA cache found for player=" + player.GetIdentity().GetName());
            EoH_Notifications.SendToPlayer(player, "LOOT CACHE INTEL", "No active DNA cache signal was found.");
            return false;
        }

        Print("[EoH_DNACacheIntel][DEBUG] Revealing REGISTERED DNA crate tier=" + best.Tier + " pos=" + best.Position.ToString() + " label=" + best.Label + " player=" + player.GetIdentity().GetName() + " playerPos=" + player.GetPosition().ToString());

        EoH_DNACacheOpenBridge.OnCrateOpenedAt(best.Tier, best.Position);
        EoH_Notifications.SendToAll("CACHE INTEL DECODED", best.Label + " was triangulated. Survivors will move on the area.");
        return true;
    }
};
