class EoH_TownRewardManager
{
    static string GetRewardContainerForTown(string townName)
    {
        int tier = GetTownTier(townName);

        if (tier >= 4)
            return "SeaChest";

        if (tier >= 3)
            return "WoodenCrate";

        return "AmmoBox";
    }

    static int GetTownTier(string townName)
    {
        if (townName == "NWAF" || townName == "Tisy" || townName == "Pavlovo Military")
            return 4;

        if (townName == "Elektro" || townName == "Chernogorsk" || townName == "Berezino")
            return 3;

        if (townName == "Vybor" || townName == "Stary Sobor" || townName == "Novy Sobor" || townName == "Zelenogorsk" || townName == "Staroye" || townName == "Polana")
            return 2;

        return 1;
    }

    static void SpawnCaptureReward(string townName, string groupName, vector townPos)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        vector pos = townPos + "3 0 3".ToVector();
        pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.05;

        ClearOldRewardCrates(pos, 8.0);

        string containerType = GetRewardContainerForTown(townName);
        EntityAI crate = EntityAI.Cast(GetGame().CreateObjectEx(containerType, pos, ECE_PLACE_ON_SURFACE));
        if (!crate)
        {
            Print("[EoH_TownReward][WARN] Failed to spawn reward crate type=" + containerType + " town=" + townName + " pos=" + pos.ToString());
            return;
        }

        crate.SetLifetime(3600);
        FillRewardCrate(crate, GetTownTier(townName));

        EoH_Notifications.SendToAll("TOWN REWARD DROPPED", "A capture cache was deployed near " + townName + " for " + groupName + ".");
        Print("[EoH_TownReward] Spawned reward crate town=" + townName + " tier=" + GetTownTier(townName).ToString() + " type=" + containerType + " pos=" + pos.ToString());
    }

    static void ClearOldRewardCrates(vector pos, float radius)
    {
        array<Object> objects = new array<Object>();
        array<CargoBase> cargos = new array<CargoBase>();
        GetGame().GetObjectsAtPosition(pos, radius, objects, cargos);

        foreach (Object obj : objects)
        {
            if (!obj)
                continue;

            string type = obj.GetType();
            if (type == "SeaChest" || type == "WoodenCrate" || type == "AmmoBox")
            {
                Print("[EoH_TownReward] Removing old reward crate type=" + type + " pos=" + obj.GetPosition().ToString());
                GetGame().ObjectDelete(obj);
            }
        }
    }

    static void FillRewardCrate(EntityAI crate, int tier)
    {
        if (!crate)
            return;

        AddItem(crate, "BandageDressing", 2 + tier);
        AddItem(crate, "Epinephrine", tier);
        AddItem(crate, "Morphine", tier);
        AddItem(crate, "AmmoBox_556x45_20Rnd", tier);
        AddItem(crate, "AmmoBox_762x39_20Rnd", tier);
        AddItem(crate, "Mag_STANAG_30Rnd", tier);
        AddItem(crate, "EoH_TownIntel", 1);

        if (tier >= 2)
        {
            AddItem(crate, "AmmoBox_308Win_20Rnd", tier);
            AddItem(crate, "Mag_AKM_30Rnd", 1);
            AddItem(crate, "EoH_TraderIntel", 1);
        }

        if (tier >= 3)
        {
            AddItem(crate, "PlateCarrierVest", 1);
            AddItem(crate, "NVGoggles", 1);
            AddItem(crate, "M4A1", 1);
        }

        if (tier >= 4)
        {
            AddItem(crate, "FAL", 1);
            AddItem(crate, "Mag_Fal_20Rnd", 2);
            AddItem(crate, "AmmoBox_308Win_20Rnd", 2);
            AddItem(crate, "EoH_TraderIntel", 1);
        }
    }

    static void AddItem(EntityAI container, string className, int count)
    {
        if (!container || className == "" || count <= 0)
            return;

        for (int i = 0; i < count; i++)
        {
            EntityAI item = container.GetInventory().CreateInInventory(className);
            if (!item)
                Print("[EoH_TownReward][WARN] Failed to add item " + className + " to reward crate.");
        }
    }
};
