class EoH_TownRewardManager
{
    protected static ref EoH_TownRewardConfig s_Config;
    protected const static string CONFIG_DIR = "$profile:EoH";
    protected const static string CONFIG_PATH = "$profile:EoH/TownRewardConfig.json";
    protected const static string PARTICLEPOINT_SMOKE = "SmokePoint_3";

    static EoH_TownRewardConfig GetConfig()
    {
        if (!s_Config)
            LoadConfig();

        return s_Config;
    }

    static void LoadConfig()
    {
        EnsureConfigDir();
        s_Config = new EoH_TownRewardConfig();

        if (!FileExist(CONFIG_PATH))
        {
            SeedDefaultConfig(s_Config);
            JsonFileLoader<EoH_TownRewardConfig>.JsonSaveFile(CONFIG_PATH, s_Config);
            Print("[EoH_TownReward] Created default config path=" + CONFIG_PATH);
        }
        else
        {
            JsonFileLoader<EoH_TownRewardConfig>.JsonLoadFile(CONFIG_PATH, s_Config);
            Print("[EoH_TownReward] Loaded config path=" + CONFIG_PATH + " towns=" + s_Config.Towns.Count().ToString() + " tiers=" + s_Config.Tiers.Count().ToString());
        }
    }

    static void EnsureConfigDir()
    {
        if (!FileExist(CONFIG_DIR))
            MakeDirectory(CONFIG_DIR);
    }

    static void ReloadConfig()
    {
        s_Config = null;
        LoadConfig();
    }

    static string GetRewardContainerForTown(string townName)
    {
        EoH_TownRewardTierConfig tierCfg = GetTierConfig(GetTownTier(townName));
        if (tierCfg && tierCfg.Container != "")
            return tierCfg.Container;

        return "AmmoBox";
    }

    static int GetTownTier(string townName)
    {
        EoH_TownRewardConfig cfg = GetConfig();
        if (cfg && cfg.Towns)
        {
            foreach (EoH_TownRewardTownConfig townCfg : cfg.Towns)
            {
                if (!townCfg)
                    continue;

                if (townCfg.TownName == townName)
                    return townCfg.Tier;
            }
        }

        return 1;
    }

    static EoH_TownRewardTierConfig GetTierConfig(int tier)
    {
        EoH_TownRewardConfig cfg = GetConfig();
        if (!cfg || !cfg.Tiers)
            return null;

        foreach (EoH_TownRewardTierConfig tierCfg : cfg.Tiers)
        {
            if (!tierCfg)
                continue;

            if (tierCfg.Tier == tier)
                return tierCfg;
        }

        return null;
    }

    static void SpawnCaptureReward(string townName, string groupName, vector townPos)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        int tier = GetTownTier(townName);
        EoH_TownRewardTierConfig tierCfg = GetTierConfig(tier);

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

        int lifetime = 3600;
        if (tierCfg && tierCfg.LifetimeSeconds > 0)
            lifetime = tierCfg.LifetimeSeconds;

        crate.SetLifetime(lifetime);
        FillRewardCrate(crate, tierCfg);
        TryAddWeekendIntelReward(crate, townName, groupName);
        SpawnRewardSmoke(pos, townName, tierCfg);

        EoH_Notifications.SendToAll("TOWN REWARD DROPPED", "A smoke-marked capture cache was deployed near " + townName + " for " + groupName + ".");
        Print("[EoH_TownReward] Spawned reward crate town=" + townName + " tier=" + tier.ToString() + " type=" + containerType + " lifetime=" + lifetime.ToString() + " pos=" + pos.ToString());
    }

    static void TryAddWeekendIntelReward(EntityAI crate, string townName, string groupName)
    {
        if (!crate)
            return;

        EoH_WeekendIntelConfigManager intelCfg = EoH_WeekendIntelConfigManager.Get();
        if (!intelCfg || !intelCfg.RollTownCaptureIntelReward())
            return;

        string intelClass = intelCfg.GetWeekendIntelClassName();
        int count = intelCfg.GetTownCaptureIntelCount();
        if (intelClass == "" || count <= 0)
            return;

        AddItem(crate, intelClass, count);
        EoH_Notifications.SendToAll("WEEKEND INTEL FOUND", groupName + " recovered encrypted weekend-event intel from the " + townName + " capture cache.");
        Print("[EoH_TownReward] Added weekend intel town=" + townName + " group=" + groupName + " item=" + intelClass + " count=" + count.ToString());
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

    static void SpawnRewardSmoke(vector cratePos, string townName, EoH_TownRewardTierConfig tierCfg)
    {
        vector smokePos = cratePos + "1.25 0 1.25".ToVector();
        smokePos[1] = GetGame().SurfaceY(smokePos[0], smokePos[2]) + 0.05;

        ClearOldRewardSmoke(smokePos, 12.0);

        EntityAI smoke = EntityAI.Cast(GetGame().CreateObjectEx("EoH_RewardSmokeMarker", smokePos, ECE_PLACE_ON_SURFACE));
        if (smoke)
        {
            smoke.SetPosition(smokePos);
            smoke.PlaceOnSurface();
            smoke.SetLifetime(900);
            Print("[EoH_TownReward] Spawned reward smoke marker town=" + townName + " type=EoH_RewardSmokeMarker pos=" + smokePos.ToString());
            return;
        }

        Print("[EoH_TownReward][WARN] Reward smoke marker failed town=" + townName + " pos=" + smokePos.ToString() + " trying ParticlePoints fallback");

        Object particleSmoke = GetGame().CreateObjectEx(PARTICLEPOINT_SMOKE, smokePos, ECE_PLACE_ON_SURFACE);
        if (!particleSmoke)
        {
            Print("[EoH_TownReward][WARN] ParticlePoints smoke fallback failed town=" + townName + " class=" + PARTICLEPOINT_SMOKE);
            return;
        }

        particleSmoke.SetPosition(smokePos);
        particleSmoke.PlaceOnSurface();

        Print("[EoH_TownReward] Spawned ParticlePoints smoke town=" + townName + " class=" + PARTICLEPOINT_SMOKE + " pos=" + smokePos.ToString());
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
            if (type.Contains("M18SmokeGrenade") || type == "EoH_RewardSmokeMarker" || type == PARTICLEPOINT_SMOKE)
            {
                Print("[EoH_TownReward] Removing old reward smoke type=" + type + " pos=" + obj.GetPosition().ToString());
                GetGame().ObjectDelete(obj);
            }
        }
    }

    static void FillRewardCrate(EntityAI crate, EoH_TownRewardTierConfig tierCfg)
    {
        if (!crate)
            return;

        if (!tierCfg || !tierCfg.Loot || tierCfg.Loot.Count() == 0)
        {
            Print("[EoH_TownReward][WARN] Missing tier loot config. Adding fallback intel only.");
            AddItem(crate, "EoH_TownIntel", 1);
            return;
        }

        foreach (EoH_TownRewardItemConfig itemCfg : tierCfg.Loot)
        {
            if (!itemCfg || itemCfg.ClassName == "")
                continue;

            float roll = Math.RandomFloat01();
            if (roll > itemCfg.Chance)
                continue;

            int minCount = Math.Max(1, itemCfg.Min);
            int maxCount = Math.Max(minCount, itemCfg.Max);
            int count = Math.RandomIntInclusive(minCount, maxCount);

            AddItem(crate, itemCfg.ClassName, count);
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

    static void SeedDefaultConfig(EoH_TownRewardConfig cfg)
    {
        if (!cfg)
            return;

        cfg.ConfigVersion = 1;
        cfg.Towns = new array<ref EoH_TownRewardTownConfig>();
        cfg.Tiers = new array<ref EoH_TownRewardTierConfig>();

        InsertTown(cfg, "Pustoshka", 1);
        InsertTown(cfg, "Mogilevka", 1);
        InsertTown(cfg, "Guglovo", 1);
        InsertTown(cfg, "Tulga", 1);
        InsertTown(cfg, "Nadezhdino", 1);
        InsertTown(cfg, "Kamenka", 1);
        InsertTown(cfg, "Vybor", 2);
        InsertTown(cfg, "Stary Sobor", 2);
        InsertTown(cfg, "Novy Sobor", 2);
        InsertTown(cfg, "Zelenogorsk", 2);
        InsertTown(cfg, "Staroye", 2);
        InsertTown(cfg, "Polana", 2);
        InsertTown(cfg, "Elektro", 3);
        InsertTown(cfg, "Chernogorsk", 3);
        InsertTown(cfg, "Berezino", 3);
        InsertTown(cfg, "NWAF", 4);
        InsertTown(cfg, "Tisy", 4);
        InsertTown(cfg, "Pavlovo Military", 4);

        EoH_TownRewardTierConfig t1 = InsertTier(cfg, 1, "AmmoBox", "EoH_RewardSmokeMarker", 3600);
        InsertLoot(t1, "BandageDressing", 2, 4, 1.0);
        InsertLoot(t1, "Epinephrine", 1, 2, 0.55);
        InsertLoot(t1, "Morphine", 1, 2, 0.45);
        InsertLoot(t1, "My_DF_Weapons_Ammo_556x45_M855", 1, 3, 0.75);
        InsertLoot(t1, "My_DF_Weapons_Ammo_762x39_PS", 1, 3, 0.70);
        InsertLoot(t1, "Mag_CMAG_30Rnd_Black", 1, 2, 0.45);
        InsertLoot(t1, "EoH_TownIntel", 1, 1, 0.18);
        InsertLoot(t1, "DNA_Keycard_Yellow", 1, 1, 0.06);

        EoH_TownRewardTierConfig t2 = InsertTier(cfg, 2, "WoodenCrate", "EoH_RewardSmokeMarker", 4500);
        InsertLoot(t2, "BandageDressing", 3, 5, 1.0);
        InsertLoot(t2, "Epinephrine", 1, 3, 0.75);
        InsertLoot(t2, "Morphine", 1, 3, 0.70);
        InsertLoot(t2, "My_DF_Weapons_Ammo_556x45_M855A1", 2, 4, 0.85);
        InsertLoot(t2, "My_DF_Weapons_Ammo_762x39_BP", 2, 4, 0.75);
        InsertLoot(t2, "My_DF_Weapons_Rifles_K416_30RndMag", 2, 3, 0.55);
        InsertLoot(t2, "My_DF_Gear_Rigs_Assault", 1, 1, 0.25);
        InsertLoot(t2, "EoH_TownIntel", 1, 1, 0.28);
        InsertLoot(t2, "EoH_TraderIntel", 1, 1, 0.15);
        InsertLoot(t2, "DNA_Keycard_Blue", 1, 1, 0.08);

        EoH_TownRewardTierConfig t3 = InsertTier(cfg, 3, "SeaChest", "EoH_RewardSmokeMarker", 5400);
        InsertLoot(t3, "Epinephrine", 2, 4, 0.85);
        InsertLoot(t3, "Morphine", 2, 4, 0.85);
        InsertLoot(t3, "SalineBagIV", 1, 2, 0.65);
        InsertLoot(t3, "My_DF_Weapons_Ammo_556x45_M995", 2, 5, 0.90);
        InsertLoot(t3, "My_DF_Weapons_Ammo_762x51_M80", 2, 5, 0.75);
        InsertLoot(t3, "My_DF_Weapons_Rifles_MK17_20RndMag", 2, 4, 0.60);
        InsertLoot(t3, "My_DF_Gear_Helmets_Fast", 1, 1, 0.30);
        InsertLoot(t3, "My_DF_Gear_Vests_PlateCarrier", 1, 1, 0.22);
        InsertLoot(t3, "EoH_TownIntel", 1, 2, 0.40);
        InsertLoot(t3, "EoH_TraderIntel", 1, 1, 0.22);
        InsertLoot(t3, "DNA_Keycard_Red", 1, 1, 0.10);

        EoH_TownRewardTierConfig t4 = InsertTier(cfg, 4, "SeaChest", "EoH_RewardSmokeMarker", 7200);
        InsertLoot(t4, "Epinephrine", 3, 5, 0.95);
        InsertLoot(t4, "Morphine", 3, 5, 0.95);
        InsertLoot(t4, "SalineBagIV", 2, 3, 0.80);
        InsertLoot(t4, "My_DF_Weapons_Ammo_338LM", 2, 5, 0.70);
        InsertLoot(t4, "My_DF_Weapons_Ammo_556x45_M995", 3, 6, 0.90);
        InsertLoot(t4, "My_DF_Weapons_Rifles_M110_20RndMag", 2, 4, 0.55);
        InsertLoot(t4, "My_DF_Gear_Helmets_Altyn", 1, 1, 0.24);
        InsertLoot(t4, "My_DF_Gear_Vests_Heavy", 1, 1, 0.18);
        InsertLoot(t4, "EoH_TownIntel", 2, 3, 0.55);
        InsertLoot(t4, "EoH_TraderIntel", 1, 2, 0.35);
        InsertLoot(t4, "DNA_Keycard_Red", 1, 1, 0.18);
        InsertLoot(t4, "PunchedCard", 1, 1, 0.12);
    }

    static void InsertTown(EoH_TownRewardConfig cfg, string townName, int tier)
    {
        EoH_TownRewardTownConfig town = new EoH_TownRewardTownConfig();
        town.TownName = townName;
        town.Tier = tier;
        cfg.Towns.Insert(town);
    }

    static EoH_TownRewardTierConfig InsertTier(EoH_TownRewardConfig cfg, int tier, string container, string smoke, int lifetime)
    {
        EoH_TownRewardTierConfig tierCfg = new EoH_TownRewardTierConfig();
        tierCfg.Tier = tier;
        tierCfg.Container = container;
        tierCfg.SmokeType = smoke;
        tierCfg.LifetimeSeconds = lifetime;
        tierCfg.Loot = new array<ref EoH_TownRewardItemConfig>();
        cfg.Tiers.Insert(tierCfg);
        return tierCfg;
    }

    static void InsertLoot(EoH_TownRewardTierConfig tierCfg, string className, int min, int max, float chance)
    {
        if (!tierCfg || !tierCfg.Loot)
            return;

        EoH_TownRewardItemConfig item = new EoH_TownRewardItemConfig();
        item.ClassName = className;
        item.Min = min;
        item.Max = max;
        item.Chance = chance;
        tierCfg.Loot.Insert(item);
    }
}
