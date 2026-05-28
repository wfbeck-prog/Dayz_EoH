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
        SpawnRewardSmoke(pos, townName, tierCfg);

        EoH_Notifications.SendToAll("TOWN REWARD DROPPED", "A smoke-marked capture cache was deployed near " + townName + " for " + groupName + ".");
        Print("[EoH_TownReward] Spawned reward crate town=" + townName + " tier=" + tier.ToString() + " type=" + containerType + " lifetime=" + lifetime.ToString() + " pos=" + pos.ToString());
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
