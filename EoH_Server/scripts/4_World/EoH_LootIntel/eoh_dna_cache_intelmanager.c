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

        EoH_DNACacheIntelTarget best = FindNearestLiveDNACrate(player.GetPosition());
        if (!best)
        {
            Print("[EoH_DNACacheIntel] No LIVE DNA crate found for player=" + player.GetIdentity().GetName());
            return false;
        }

        Print("[EoH_DNACacheIntel][DEBUG] Revealing LIVE DNA crate tier=" + best.Tier + " pos=" + best.Position.ToString() + " label=" + best.Label + " player=" + player.GetIdentity().GetName() + " playerPos=" + player.GetPosition().ToString());

        EoH_DNACacheOpenBridge.OnCrateOpenedAt(best.Tier, best.Position);
        EoH_Notifications.SendToAll("CACHE INTEL DECODED", best.Label + " was triangulated. Survivors will move on the area.");
        return true;
    }

    protected EoH_DNACacheIntelTarget FindNearestLiveDNACrate(vector playerPos)
    {
        array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition3D(playerPos, 16000.0, objects, null);

        EoH_DNACacheIntelTarget best = null;
        float bestDistance = 99999999.0;

        foreach (Object obj : objects)
        {
            if (!obj)
                continue;

            string tier = GetTierFromObject(obj);
            if (tier == string.Empty || tier == "Purple")
                continue;

            vector pos = obj.GetPosition();
            float dist = vector.Distance(playerPos, pos);
            if (dist < bestDistance)
            {
                bestDistance = dist;
                best = new EoH_DNACacheIntelTarget(tier, pos, tier + " DNA Cache Signal");
            }
        }

        if (best)
            Print("[EoH_DNACacheIntel][DEBUG] Nearest LIVE DNA crate selected tier=" + best.Tier + " dist=" + bestDistance.ToString() + " pos=" + best.Position.ToString());

        return best;
    }

    protected string GetTierFromObject(Object obj)
    {
        if (!obj)
            return string.Empty;

        string type = obj.GetType();

        if (!type.Contains("DNA") && !type.Contains("dna"))
            return string.Empty;

        if (!type.Contains("Crate") && !type.Contains("crate"))
            return string.Empty;

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
};
