class EoH_TownAIManager
{
    protected static ref EoH_TownAIManager s_Instance;
    protected ref EoH_TownAIConfig m_Config;
    protected ref map<string, ref EoH_TownAIActiveTown> m_ActiveTowns;
    protected int m_LastTick;
    protected int m_HeartbeatCount;

    protected const string CONFIG_DIR = "$profile:EoH";
    protected const string CONFIG_PATH = "$profile:EoH/TownAIConfig.json";

    void EoH_TownAIManager()
    {
        m_LastTick = 0;
        m_HeartbeatCount = 0;
        m_ActiveTowns = new map<string, ref EoH_TownAIActiveTown>();
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
            NormalizeConfig();
            Print("[EoH_TownAI] Loaded config path=" + CONFIG_PATH + " enabled=" + m_Config.Enabled.ToString() + " towns=" + m_Config.Towns.Count().ToString() + " tiers=" + m_Config.Tiers.Count().ToString() + " requirePlayerNearby=" + m_Config.RequirePlayerNearby.ToString() + " activationRadius=" + m_Config.ActivationRadius.ToString());
        }
    }

    void NormalizeConfig()
    {
        if (!m_Config)
            return;

        if (m_Config.ActivationRadius <= 0)
            m_Config.ActivationRadius = 1000.0;

        if (m_Config.MaxActiveTowns <= 0)
            m_Config.MaxActiveTowns = 8;
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

        if (!m_Config)
        {
            Print("[EoH_TownAI][HEARTBEAT] Tick skipped: config is null.");
            return;
        }

        m_HeartbeatCount++;
        if (m_HeartbeatCount <= 3 || m_HeartbeatCount % 6 == 0)
            Print("[EoH_TownAI][HEARTBEAT] Tick received enabled=" + m_Config.Enabled.ToString() + " tickSeconds=" + m_Config.TickSeconds.ToString() + " towns=" + m_Config.Towns.Count().ToString() + " tiers=" + m_Config.Tiers.Count().ToString() + " active=" + m_ActiveTowns.Count().ToString());

        if (!m_Config.Enabled)
            return;

        MonitorOwnerFriendlyPlayers();

        int now = GetGame().GetTime();
        int intervalMs = Math.Max(10, m_Config.TickSeconds) * 1000;
        if (m_LastTick > 0 && now - m_LastTick < intervalMs)
            return;

        m_LastTick = now;
        EvaluateTownAI();
    }

    void MonitorOwnerFriendlyPlayers()
    {
        if (!m_ActiveTowns)
            return;

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            for (int i = 0; i < m_ActiveTowns.Count(); i++)
            {
                EoH_TownAIActiveTown active = m_ActiveTowns.GetElement(i);
                if (!active || active.OwnerGroupID == "")
                    continue;

                if (!IsOwnerFriendly(player, active.TownName))
                    continue;

                vector townPos = GetTownPosition(active.TownName);
                float dist = vector.Distance(player.GetPosition(), townPos);
                if (dist > 250.0)
                    continue;

                foreach (Object obj : active.SpawnedObjects)
                {
                    eAIBase ai = eAIBase.Cast(obj);
                    if (!ai)
                        continue;

                    ai.eAI_SetThreatDistanceLimit(5.0);
                    ai.EoH_IsFriendlyTownOwner(player);
                }
            }
        }
    }

    void EvaluateTownAI()
    {
        if (!m_Config || !m_Config.Towns)
            return;

        ref array<ref EoH_TownAITownConfig> ordered = BuildPriorityTownList();
        int activeEligible = 0;
        ref array<string> eligibleNames = new array<string>();

        foreach (EoH_TownAITownConfig townCfg : ordered)
        {
            if (!townCfg || !townCfg.Enabled)
                continue;

            vector townPos = GetTownPosition(townCfg.TownName);
            bool playerNearby = IsPlayerNearPosition(townPos, m_Config.ActivationRadius);
            if (m_Config.RequirePlayerNearby && !playerNearby)
            {
                Print("[EoH_TownAI][PROXIMITY] Skipping town=" + townCfg.TownName + " no players within " + m_Config.ActivationRadius.ToString() + "m");
                continue;
            }

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
            eligibleNames.Insert(townCfg.TownName);

            Print("[EoH_TownAI][DRYRUN] town=" + townCfg.TownName + " tier=" + townCfg.Tier.ToString() + " owner=" + owner + " playerNearby=" + playerNearby.ToString() + " patrols=" + tierCfg.MinPatrols.ToString() + "-" + tierCfg.MaxPatrols.ToString() + " camps=" + tierCfg.MinCamps.ToString() + "-" + tierCfg.MaxCamps.ToString());

            EnsureActiveTown(townCfg, tierCfg);
        }

        CleanupInactiveTowns(eligibleNames);
        Print("[EoH_TownAI][DRYRUN] Eligible active towns this tick=" + activeEligible.ToString() + " max=" + m_Config.MaxActiveTowns.ToString() + " tracked=" + m_ActiveTowns.Count().ToString());
    }

    bool IsPlayerNearPosition(vector pos, float radius)
    {
        if (pos == "0 0 0".ToVector())
            return false;

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player)
                continue;

            if (vector.Distance(player.GetPosition(), pos) <= radius)
                return true;
        }

        return false;
    }

    ref array<ref EoH_TownAITownConfig> BuildPriorityTownList()
    {
        ref array<ref EoH_TownAITownConfig> ordered = new array<ref EoH_TownAITownConfig>();

        foreach (EoH_TownAITownConfig ownedTown : m_Config.Towns)
        {
            if (!ownedTown)
                continue;

            string owner = GetTownOwner(ownedTown.TownName);
            if (owner != "" && owner != "Unclaimed")
                ordered.Insert(ownedTown);
        }

        foreach (EoH_TownAITownConfig unownedTown : m_Config.Towns)
        {
            if (!unownedTown)
                continue;

            string unownedOwner = GetTownOwner(unownedTown.TownName);
            if (unownedOwner == "" || unownedOwner == "Unclaimed")
                ordered.Insert(unownedTown);
        }

        return ordered;
    }

    void EnsureActiveTown(EoH_TownAITownConfig townCfg, EoH_TownAITierConfig tierCfg)
    {
        if (!townCfg || !tierCfg)
            return;

        EoH_TownAIActiveTown active;
        if (m_ActiveTowns.Find(townCfg.TownName, active) && active)
        {
            RefreshOwnerIdentity(active);

            if (!HasLiveTrackedObjects(active))
                SpawnTownPatrol(active, townCfg, tierCfg);
            else
                Print("[EoH_TownAI][TRACK] town=" + townCfg.TownName + " already tracked spawnedObjects=" + active.SpawnedObjects.Count().ToString() + " owner=" + active.OwnerGroupName);

            return;
        }

        active = new EoH_TownAIActiveTown();
        active.TownName = townCfg.TownName;
        active.Tier = townCfg.Tier;
        active.LastSpawnTime = GetGame().GetTime();
        RefreshOwnerIdentity(active);
        m_ActiveTowns.Set(townCfg.TownName, active);

        Print("[EoH_TownAI][TRACK] town=" + townCfg.TownName + " tier=" + townCfg.Tier.ToString() + " owner=" + active.OwnerGroupName + " entered active tracking. Spawning conservative patrol.");
        SpawnTownPatrol(active, townCfg, tierCfg);
    }

    void RefreshOwnerIdentity(EoH_TownAIActiveTown active)
    {
        if (!active)
            return;

        EoH_WorldStateTownState state = EoH_WorldStateManager.Get().GetTownState(active.TownName);
        if (!state || state.OwnerGroupName == "" || state.OwnerGroupName == "Unclaimed")
        {
            active.OwnerGroupID = "";
            active.OwnerGroupName = "Unclaimed";
            return;
        }

        active.OwnerGroupID = state.OwnerGroupID;
        active.OwnerGroupName = state.OwnerGroupName;
    }

    bool IsOwnerFriendly(PlayerBase player, string townName)
    {
        if (!player || townName == "")
            return false;

        EoH_TownAIActiveTown active;
        if (!m_ActiveTowns.Find(townName, active) || !active)
            return false;

        if (active.OwnerGroupID == "")
            return false;

        string playerGroupID = EoH_GroupHelper.GetGroupID(player);
        return playerGroupID != "" && playerGroupID == active.OwnerGroupID;
    }

    bool HasLiveTrackedObjects(EoH_TownAIActiveTown active)
    {
        if (!active || !active.SpawnedObjects)
            return false;

        for (int i = active.SpawnedObjects.Count() - 1; i >= 0; i--)
        {
            Object obj = active.SpawnedObjects.Get(i);
            if (obj)
                return true;

            active.SpawnedObjects.Remove(i);
        }

        return false;
    }

    void SpawnTownPatrol(EoH_TownAIActiveTown active, EoH_TownAITownConfig townCfg, EoH_TownAITierConfig tierCfg)
    {
        if (!active || !townCfg || !tierCfg)
            return;

        vector center = GetTownPosition(townCfg.TownName);
        if (center == "0 0 0".ToVector())
        {
            Print("[EoH_TownAI][SPAWN][WARN] No valid town position for " + townCfg.TownName);
            return;
        }

        int minCount = Math.Max(1, tierCfg.MinAIPerPatrol);
        int maxCount = Math.Max(minCount, tierCfg.MaxAIPerPatrol);
        int count = Math.RandomIntInclusive(minCount, maxCount);
        if (townCfg.Tier == 1)
            count = Math.Min(count, 2);
        else if (townCfg.Tier == 2)
            count = Math.Min(count, 3);
        else if (townCfg.Tier == 3)
            count = Math.Min(count, 4);
        else
            count = Math.Min(count, 5);

        string loadout = PickPatrolLoadout(tierCfg);
        eAIGroup group = EoH_TownAISpawnAdapter.CreateTownPatrolGroup(center);

        Print("[EoH_TownAI][SPAWN] Spawning patrol town=" + townCfg.TownName + " tier=" + townCfg.Tier.ToString() + " owner=" + active.OwnerGroupName + " count=" + count.ToString() + " loadout=" + loadout);

        for (int i = 0; i < count; i++)
        {
            vector spawnPos = GetRandomSpawnPosition(center, tierCfg.SpawnRadiusMin, tierCfg.SpawnRadiusMax);
            Object obj = EoH_TownAISpawnAdapter.SpawnTownPatrolUnit(townCfg.TownName, spawnPos, center, loadout, group);
            if (obj)
                active.SpawnedObjects.Insert(obj);
        }

        Print("[EoH_TownAI][TRACK] town=" + townCfg.TownName + " owner=" + active.OwnerGroupName + " registered patrol objects=" + active.SpawnedObjects.Count().ToString());
    }

    string PickPatrolLoadout(EoH_TownAITierConfig tierCfg)
    {
        if (!tierCfg || !tierCfg.Loadouts || !tierCfg.Loadouts.PatrolLoadouts || tierCfg.Loadouts.PatrolLoadouts.Count() == 0)
            return "EoH_AI_Patrol_Assault_DF";

        int index = Math.RandomInt(0, tierCfg.Loadouts.PatrolLoadouts.Count());
        return tierCfg.Loadouts.PatrolLoadouts.Get(index);
    }

    vector GetTownPosition(string townName)
    {
        vector pos = EoH_CaptureManager.Get().GetTownPos(townName);
        if (pos != "0 0 0".ToVector())
            return pos;

        Print("[EoH_TownAI][SPAWN][WARN] Capture manager returned no position for town=" + townName);
        return "0 0 0".ToVector();
    }

    vector GetRandomSpawnPosition(vector center, float minRadius, float maxRadius)
    {
        float radius = Math.RandomFloatInclusive(Math.Max(10.0, minRadius), Math.Max(minRadius + 1.0, maxRadius));
        float angle = Math.RandomFloatInclusive(0.0, Math.PI2);
        vector pos = center;
        pos[0] = center[0] + Math.Cos(angle) * radius;
        pos[2] = center[2] + Math.Sin(angle) * radius;
        pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.05;
        return pos;
    }

    void CleanupInactiveTowns(array<string> eligibleNames)
    {
        if (!m_ActiveTowns)
            return;

        ref array<string> removeList = new array<string>();

        for (int i = 0; i < m_ActiveTowns.Count(); i++)
        {
            string townName = m_ActiveTowns.GetKey(i);
            if (eligibleNames.Find(townName) == -1)
                removeList.Insert(townName);
        }

        foreach (string removeTown : removeList)
        {
            CleanupTown(removeTown);
        }
    }

    void CleanupTown(string townName)
    {
        EoH_TownAIActiveTown active;
        if (!m_ActiveTowns.Find(townName, active) || !active)
            return;

        foreach (Object obj : active.SpawnedObjects)
        {
            if (obj)
                GetGame().ObjectDelete(obj);
        }

        m_ActiveTowns.Remove(townName);
        Print("[EoH_TownAI][TRACK] town=" + townName + " removed from active tracking.");
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

        cfg.ConfigVersion = 2;
        cfg.Enabled = false;
        cfg.TickSeconds = 60;
        cfg.MaxActiveTowns = 8;
        cfg.RespawnCooldownSeconds = 1800;
        cfg.RequirePlayerNearby = true;
        cfg.ActivationRadius = 1000.0;

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
