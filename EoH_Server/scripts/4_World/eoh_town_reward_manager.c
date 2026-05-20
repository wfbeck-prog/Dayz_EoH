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

        vector pos = FindRewardSpawnPosition(townPos);
        ClearOldRewardCrates(pos, 10.0);

        string containerType = GetRewardContainerForTown(townName);
        EntityAI crate = EntityAI.Cast(GetGame().CreateObjectEx(containerType, pos, ECE_PLACE_ON_SURFACE));
        if (!crate)
        {
            Print("[EoH_TownReward][WARN] Failed to spawn reward crate type=" + containerType + " town=" + townName + " pos=" + pos.ToString());
            return;
        }

        crate.SetPosition(pos);
        crate.PlaceOnSurface();
        crate.SetLifetime(3600);
        FillRewardCrate(crate, GetTownTier(townName));
        SpawnRewardSmoke(pos, townName);

        EoH_Notifications.SendToAll("TOWN REWARD DROPPED", "A smoke-marked capture cache was deployed near " + townName + " for " + groupName + ".");
        Print("[EoH_TownReward] Spawned reward crate town=" + townName + " tier=" + GetTownTier(townName).ToString() + " type=" + containerType + " pos=" + pos.ToString());
    }

    static vector FindRewardSpawnPosition(vector townPos)
    {
        vector offsets[8];
        offsets[0] = "3 0 3".ToVector();
        offsets[1] = "5 0 0".ToVector();
        offsets[2] = "0 0 5".ToVector();
        offsets[3] = "-5 0 0".ToVector();
        offsets[4] = "0 0 -5".ToVector();
        offsets[5] = "7 0 7".ToVector();
        offsets[6] = "-7 0 7".ToVector();
        offsets[7] = "7 0 -7".ToVector();

        for (int i = 0; i < 8; i++)
        {
            vector candidate = townPos + offsets[i];
            candidate[1] = GetGame().SurfaceY(candidate[0], candidate[2]) + 0.10;

            if (candidate[1] > 0.1)
                return candidate;
        }

        vector fallback = townPos;
        fallback[1] = GetGame().SurfaceY(fallback[0], fallback[2]) + 0.10;
        return fallback;
    }

    static void SpawnRewardSmoke(vector cratePos, string townName)
    {
        vector smokePos = cratePos + "1.25 0 1.25".ToVector();
        smokePos[1] = GetGame().SurfaceY(smokePos[0], smokePos[2]) + 0.05;

        ClearOldRewardSmoke(smokePos, 12.0);

        EntityAI smoke = EntityAI.Cast(GetGame().CreateObjectEx("M18SmokeGrenade_Red", smokePos, ECE_PLACE_ON_SURFACE));
        if (!smoke)
        {
            Print("[EoH_TownReward][WARN] Failed to spawn reward smoke town=" + townName + " pos=" + smokePos.ToString());
            return;
        }

        smoke.SetPosition(smokePos);
        smoke.PlaceOnSurface();
        smoke.SetLifetime(900);

        // Try to activate smoke. Some builds expose smoke through different inherited methods,
        // so leave the spawned grenade in world even if activation is handled by vanilla state.
        SmokeGrenadeBase smokeBase = SmokeGrenadeBase.Cast(smoke);
        if (smokeBase)
        {
            smokeBase.SetTakeable(false);
            smokeBase.StartSmoke();
        }

        Print("[EoH_TownReward] Spawned reward smoke town=" + townName + " pos=" + smokePos.ToString());
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

    static void ClearOldRewardSmoke(vector pos, float radius)
    {
        array<Object> objects = new array<Object>();
        array<CargoBase> cargos = new array<CargoBase>();
        GetGame().GetObjectsAtPosition(pos, radius, objects, cargos);

        foreach (Object obj : objects)
        {
            if (!obj)
                continue;

            string type = obj.GetType();
            if (type.Contains("M18SmokeGrenade"))
            {
                Print("[EoH_TownReward] Removing old reward smoke type=" + type + " pos=" + obj.GetPosition().ToString());
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
        AddItem(crate, "My_DF_Weapons_Ammo_556x45_M855A1", tier);
        AddItem(crate, "My_DF_Weapons_Ammo_762x39_BP", tier);
        AddItem(crate, "My_DF_Weapons_Rifle_M4A1_30RndMag", tier);
        AddItem(crate, "EoH_TownIntel", 1);

        if (tier >= 2)
        {
            AddItem(crate, "My_DF_Weapons_Ammo_762x51_M80", tier);
            AddItem(crate, "My_DF_Weapons_Rifle_AK12_30RndMag", 1);
            AddItem(crate, "EoH_TraderIntel", 1);
        }

        if (tier >= 3)
        {
            AddItem(crate, "My_DF_Gear_Armors_Standard", 1);
            AddItem(crate, "My_DF_Gear_Heads_H70_NVG", 1);
            AddItem(crate, "My_DF_Weapons_Rifle_M4A1", 1);
        }

        if (tier >= 4)
        {
            AddItem(crate, "My_DF_Weapons_DMR_M14", 1);
            AddItem(crate, "My_DF_Weapons_DMR_M14_20RndMag", 2);
            AddItem(crate, "My_DF_Weapons_Ammo_762x51_M80", 2);
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
