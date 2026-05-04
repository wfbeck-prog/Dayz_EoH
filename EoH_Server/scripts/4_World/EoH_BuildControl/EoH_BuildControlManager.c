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

        // Require group (your rule)
        string groupID = EoH_GroupHelper.GetGroupID(player);
        if (groupID == "")
        {
            Notify(player, "You must be in a group to build. Create one even if solo.");
            return false;
        }

        if (IsBlacklisted(cfg, itemType))
        {
            Notify(player, cfg.BlacklistMessage);
            return false;
        }

        bool inTerritory = IsInsideOwnedTerritory(cfg, player, position);

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
                Notify(player, "Your group does not control this territory.");
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
            int count = CountGroupObjects(player, itemType, position, rule.CountRadius);
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

    static bool IsInsideOwnedTerritory(EoH_BuildControlConfig cfg, PlayerBase player, vector pos)
    {
        array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition(pos, cfg.TerritoryRadiusMeters, objects, null);

        string groupID = EoH_GroupHelper.GetGroupID(player);

        foreach (Object obj : objects)
        {
            TerritoryFlag flag = TerritoryFlag.Cast(obj);
            if (flag && flag.GetEoHOwner() == groupID)
                return true;
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
            TerritoryFlag flag = TerritoryFlag.Cast(obj);
            if (flag)
                return false;
        }

        return true;
    }

    static int CountGroupObjects(PlayerBase player, string typeName, vector pos, float radius)
    {
        string groupID = EoH_GroupHelper.GetGroupID(player);

        array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition(pos, radius, objects, null);

        int count = 0;

        foreach (Object obj : objects)
        {
            if (obj && obj.GetType() == typeName)
            {
                TerritoryFlag flag = TerritoryFlag.Cast(obj);
                if (flag && flag.GetEoHOwner() == groupID)
                    count++;
            }
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
