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
    protected ref array<ref EoH_DNACacheIntelTarget> m_Targets;

    static EoH_DNACacheIntelManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_DNACacheIntelManager();
        return s_Instance;
    }

    void EoH_DNACacheIntelManager()
    {
        m_Targets = new array<ref EoH_DNACacheIntelTarget>();
        InitDefaults();
    }

    void InitDefaults()
    {
        m_Targets.Clear();

        // Temporary fallback targets until DNA cache positions are moved into a dedicated config.
        // These are broad map-center style hints and can be replaced with exact cache positions later.
        m_Targets.Insert(new EoH_DNACacheIntelTarget(EoH_DNACacheTier.YELLOW, "7200 0 8400".ToVector(), "Yellow DNA Cache Signal"));
        m_Targets.Insert(new EoH_DNACacheIntelTarget(EoH_DNACacheTier.GREEN, "7200 0 8400".ToVector(), "Green DNA Cache Signal"));
        m_Targets.Insert(new EoH_DNACacheIntelTarget(EoH_DNACacheTier.BLUE, "7200 0 8400".ToVector(), "Blue DNA Cache Signal"));
        m_Targets.Insert(new EoH_DNACacheIntelTarget(EoH_DNACacheTier.RED, "7200 0 8400".ToVector(), "Red DNA Cache Signal"));
    }

    bool RevealNearestCacheToPlayer(PlayerBase player)
    {
        if (!GetGame().IsServer() || !player || !player.GetIdentity())
            return false;

        if (!m_Targets || m_Targets.Count() == 0)
            InitDefaults();

        EoH_DNACacheIntelTarget best = null;
        float bestDistance = 999999.0;
        vector playerPos = player.GetPosition();

        foreach (EoH_DNACacheIntelTarget target : m_Targets)
        {
            if (!target)
                continue;

            float dist = vector.Distance(playerPos, target.Position);
            if (dist < bestDistance)
            {
                bestDistance = dist;
                best = target;
            }
        }

        if (!best)
        {
            Print("[EoH_DNACacheIntel] No cache intel target found for player=" + player.GetIdentity().GetName());
            return false;
        }

        EoH_DNACacheOpenBridge.OnCrateOpenedAt(best.Tier, best.Position);
        EoH_Notifications.SendToAll("CACHE INTEL DECODED", best.Label + " was triangulated. Survivors will move on the area.");
        Print("[EoH_DNACacheIntel] Revealed cache intel tier=" + best.Tier + " pos=" + best.Position.ToString() + " by=" + player.GetIdentity().GetName());
        return true;
    }
};
