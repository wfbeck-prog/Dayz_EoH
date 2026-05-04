class EoH_BuildControlManager
{
    static bool CanPlace(PlayerBase player, string itemType, vector position)
    {
        EoH_BuildControlConfig cfg = GetEoHBuildControlConfig();

        if (!cfg || !cfg.Enabled)
            return true;

        string steamId = "";
        if (player && player.GetIdentity())
            steamId = player.GetIdentity().GetPlainId();

        if (cfg.IsAdmin(steamId))
            return true;

        if (IsBlacklisted(cfg, itemType))
        {
            Notify(player, cfg.BlacklistMessage);
            return false;
        }

        bool inTerritory = IsInsideTerritory(cfg, position);

        EoH_BuildControlRule rule = cfg.FindRule(itemType);

        if (rule && rule.BlockPlacement)
        {
            Notify(player, rule.DenyMessage);
            return false;
        }

        if (cfg.RequireTerritoryForBuilding && !inTerritory)
        {
            if (!rule || !rule.IgnoreTerritoryCheck)
            {
                Notify(player, cfg.TerritoryRequiredMessage);
                return false;
            }
        }

        if (rule && rule.RequiresTerritory && !inTerritory)
        {
            Notify(player, rule.DenyMessage);
            return false;
        }

        if (cfg.EnforcePerPlayerLimits && rule && rule.MaxPlacedPerPlayer > -1)
        {
            int count = CountNearby(player, itemType, position, rule.CountRadius);
            if (count >= rule.MaxPlacedPerPlayer)
            {
                Notify(player, cfg.LimitReachedMessage);
                return false;
            }
        }

        if (!CheckTerritoryConflict(cfg, itemType, position))
        {
            Notify(player, cfg.TerritoryConflictMessage);
            return false;
        }

        return true;
    }

    static bool IsBlacklisted(EoH_BuildControlConfig cfg, string typeName)
    {
        return cfg.BuildBlacklist.Find(typeName) >= 0;
    }

    static bool IsInsideTerritory(EoH_BuildControlConfig cfg, vector pos)
    {
        array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition(pos, cfg.TerritoryRadiusMeters, objects, null);

        foreach (Object obj : objects)
        {
            foreach (string cls : cfg.TerritoryAnchorClasses)
            {
                if (obj && obj.IsKindOf(cls))
                    return true;
            }
        }

        return false;
    }

    static bool CheckTerritoryConflict(EoH_BuildControlConfig cfg, string itemType, vector pos)
    {
        if (itemType != "TerritoryFlagKit")
            return true;

        array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition(pos, cfg.TerritoryConflictRadiusMeters, objects, null);

        foreach (Object obj : objects)
        {
            foreach (string cls : cfg.TerritoryAnchorClasses)
            {
                if (obj && obj.IsKindOf(cls))
                    return false;
            }
        }

        return true;
    }

    static int CountNearby(PlayerBase player, string typeName, vector pos, float radius)
    {
        array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition(pos, radius, objects, null);

        int count = 0;
        foreach (Object obj : objects)
        {
            if (obj && obj.GetType() == typeName)
                count++;
        }

        return count;
    }

    static void Notify(PlayerBase player, string msg)
    {
        if (!player)
            return;

        player.MessageStatus(msg);
    }
};
