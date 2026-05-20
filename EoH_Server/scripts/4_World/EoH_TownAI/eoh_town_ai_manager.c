class EoH_TownAIManager
{
    protected static ref EoH_TownAIManager s_Instance;
    protected ref EoH_TownAIConfig m_Config;
    protected int m_LastTick;

    protected const string CONFIG_DIR = "$profile:EoH";
    protected const string CONFIG_PATH = "$profile:EoH/TownAIConfig.json";

    void EoH_TownAIManager()
    {
        m_LastTick = 0;
        LoadConfig();
    }

    static EoH_TownAIManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_TownAIManager();

        return s_Instance;
    }

    void LoadConfig()
    {
        EnsureConfigDir();
        m_Config = new EoH_TownAIConfig();

        if (!FileExist(CONFIG_PATH))
        {
            SeedDefaultConfig(m_Config);
            JsonFileLoader<EoH_TownAIConfig>.JsonSaveFile(CONFIG_PATH, m_Config);
            Print("[EoH_TownAI] Created default config path=" + CONFIG_PATH + " enabled=" + m_Config.Enabled.ToString());
        }
        else
        {
            JsonFileLoader<EoH_TownAIConfig>.JsonLoadFile(CONFIG_PATH, m_Config);
            Print("[EoH_TownAI] Loaded config path=" + CONFIG_PATH + " enabled=" + m_Config.Enabled.ToString() + " towns=" + m_Config.Towns.Count().ToString() + " tiers=" + m_Config.Tiers.Count().ToString());
        }
    }

    void EnsureConfigDir()
    {
        if (!FileExist(CONFIG_DIR))
            MakeDirectory(CONFIG_DIR);
    }

    void ReloadConfig()
    {
        m_Config = null;
        LoadConfig();
    }

    void Tick()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!m_Config)
            LoadConfig();

        if (!m_Config || !m_Config.Enabled)
            return;

        int now = GetGame().GetTime();
        int intervalMs = Math.Max(10, m_Config.TickSeconds) * 1000;
        if (m_LastTick > 0 && now - m_LastTick < intervalMs)
            return;

        m_LastTick = now;
        EvaluateTownAI();
    }

    void EvaluateTownAI()
    {
        if (!m_Config || !m_Config.Towns)
            return;

        int activeEligible = 0;

        foreach (EoH_TownAITownConfig townCfg : m_Config.Towns)
        {
            if (!townCfg || !townCfg.Enabled)
                continue;

            if (activeEligible >= m_Config.MaxActiveTowns)
                break;

            string owner = GetTownOwner(townCfg.TownName);
            bool unclaimed = owner == "" || owner == "Unclaimed";
            bool eligible = false;

            if (unclaimed && townCfg.SpawnWhenUnclaimed)
                eligible = true;
            else if (!unclaimed && townCfg.SpawnWhenOwned)
                eligible = true;

            if (!eligible)
                continue;

            EoH_TownAITierConfig tierCfg = GetTierConfig(townCfg.Tier);
            if (!tierCfg || !tierCfg.Enabled)
                continue;

            activeEligible++;
            Print("[EoH_TownAI][DRYRUN] town=" + townCfg.TownName + " tier=" + townCfg.Tier.ToString() + " owner=" + owner + " patrols=" + tierCfg.MinPatrols.ToString() + "-" + tierCfg.MaxPatrols.ToString() + " camps=" + tierCfg.MinCamps.ToString() + "-" + tierCfg.MaxCamps.ToString());
        }

        Print("[EoH_TownAI][DRYRUN] Eligible active towns this tick=" + activeEligible.ToString() + " max=" + m_Config.MaxActiveTowns.ToString());
    }

    string GetTownOwner(string townName)
    {
        EoH_WorldStateTownState state = EoH_WorldStateManager.Get().GetTownState(townName);
        if (!state || state.OwnerGroupName == "")
            return "Unclaimed";

        return state.OwnerGroupName;
    }

    EoH_TownAITierConfig GetTierConfig(int tier)
    {
        if (!m_Config || !m_Config.Tiers)
            return null;

        foreach (EoH_TownAITierConfig tierCfg : m_Config.Tiers)
        {
            if (!tierCfg)
                continue;

            if (tierCfg.Tier == tier)
                return tierCfg;
        }

        return null;
    }

    void SeedDefaultConfig(EoH_TownAIConfig cfg)
    {
        if (!cfg)
            return;

        cfg.ConfigVersion = 1;
        cfg.Enabled = false;
        cfg.TickSeconds = 60;
        cfg.MaxActiveTowns = 4;
        cfg.RespawnCooldownSeconds = 1800;

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

        EoH_TownAITierConfig t1 = InsertTier(cfg, 1, 0, 1, 0, 0, 1, 2, 0, 0, 90.0, 180.0, 550.0);
        InsertPatrolLoadout(t1, "EoH_AI_Patrol_Scout_DF");
        InsertPatrolLoadout(t1, "EoH_AI_Patrol_Assault_DF");

        EoH_TownAITierConfig t2 = InsertTier(cfg, 2, 1, 1, 0, 1, 2, 3, 2, 3, 100.0, 220.0, 650.0);
        InsertPatrolLoadout(t2, "EoH_AI_Patrol_Assault_DF");
        InsertPatrolLoadout(t2, "EoH_AI_Patrol_Scout_DF");
        InsertCampLoadout(t2, "EoH_AI_Camp_Guard_DF");

        EoH_TownAITierConfig t3 = InsertTier(cfg, 3, 1, 2, 1, 1, 2, 4, 3, 4, 120.0, 260.0, 750.0);
        InsertPatrolLoadout(t3, "EoH_AI_Patrol_Assault_DF");
        InsertPatrolLoadout(t3, "EoH_AI_Patrol_Marksman_DF");
        InsertCampLoadout(t3, "EoH_AI_Camp_Guard_DF");
        InsertCampLoadout(t3, "EoH_AI_Camp_Raider_DF");

        EoH_TownAITierConfig t4 = InsertTier(cfg, 4, 2, 3, 1, 2, 3, 5, 4, 6, 140.0, 320.0, 900.0);
        InsertPatrolLoadout(t4, "EoH_AI_Patrol_Assault_DF");
        InsertPatrolLoadout(t4, "EoH_AI_Patrol_Marksman_DF");
        InsertCampLoadout(t4, "EoH_AI_Camp_Heavy_DF");
        InsertCampLoadout(t4, "EoH_AI_Camp_Raider_DF");
    }

    void InsertTown(EoH_TownAIConfig cfg, string townName, int tier)
    {
        EoH_TownAITownConfig town = new EoH_TownAITownConfig();
        town.TownName = townName;
        town.Tier = tier;
        cfg.Towns.Insert(town);
    }

    EoH_TownAITierConfig InsertTier(EoH_TownAIConfig cfg, int tier, int minPatrols, int maxPatrols, int minCamps, int maxCamps, int minAIPerPatrol, int maxAIPerPatrol, int minAIPerCamp, int maxAIPerCamp, float spawnRadiusMin, float spawnRadiusMax, float despawnRadius)
    {
        EoH_TownAITierConfig tierCfg = new EoH_TownAITierConfig();
        tierCfg.Tier = tier;
        tierCfg.Enabled = true;
        tierCfg.MinPatrols = minPatrols;
        tierCfg.MaxPatrols = maxPatrols;
        tierCfg.MinCamps = minCamps;
        tierCfg.MaxCamps = maxCamps;
        tierCfg.MinAIPerPatrol = minAIPerPatrol;
        tierCfg.MaxAIPerPatrol = maxAIPerPatrol;
        tierCfg.MinAIPerCamp = minAIPerCamp;
        tierCfg.MaxAIPerCamp = maxAIPerCamp;
        tierCfg.SpawnRadiusMin = spawnRadiusMin;
        tierCfg.SpawnRadiusMax = spawnRadiusMax;
        tierCfg.DespawnRadius = despawnRadius;
        cfg.Tiers.Insert(tierCfg);
        return tierCfg;
    }

    void InsertPatrolLoadout(EoH_TownAITierConfig tierCfg, string loadout)
    {
        if (tierCfg && tierCfg.Loadouts && loadout != "")
            tierCfg.Loadouts.PatrolLoadouts.Insert(loadout);
    }

    void InsertCampLoadout(EoH_TownAITierConfig tierCfg, string loadout)
    {
        if (tierCfg && tierCfg.Loadouts && loadout != "")
            tierCfg.Loadouts.CampLoadouts.Insert(loadout);
    }
};
