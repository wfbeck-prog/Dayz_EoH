class EoH_AIBDynamicGenerator
{
    protected static const string EOH_AIB_GENERATED_FILE = "$profile:EoH\\WorldState\\EoH_DynamicAIB_Generated.json";

    // Your AI Bandits config folder from HostHavoc: profile/AI_Bandits/DynamicAIB.json
    protected static const string EOH_AIB_LIVE_FILE = "$profile:AI_Bandits\\DynamicAIB.json";

    protected static const string EOH_AIB_BACKUP_FILE = "$profile:EoH\\WorldState\\DynamicAIB_Backup_Before_EoH_Merge.json";

    static void GenerateFromWorldState(EoH_WorldStateData state)
    {
        EoH_AIBDynamicConfig generated = BuildGeneratedConfig(state);
        if (!generated)
            return;

        JsonFileLoader<EoH_AIBDynamicConfig>.JsonSaveFile(EOH_AIB_GENERATED_FILE, generated);
        Print("[EoH_AIBGenerator] Generated AI Bandits config: " + EOH_AIB_GENERATED_FILE);
    }

    static void GenerateAndMergeIntoLiveDynamicAIB(EoH_WorldStateData state)
    {
        EoH_AIBDynamicConfig generated = BuildGeneratedConfig(state);
        if (!generated)
            return;

        JsonFileLoader<EoH_AIBDynamicConfig>.JsonSaveFile(EOH_AIB_GENERATED_FILE, generated);

        EoH_AIBDynamicConfig live = new EoH_AIBDynamicConfig();

        if (FileExist(EOH_AIB_LIVE_FILE))
        {
            JsonFileLoader<EoH_AIBDynamicConfig>.JsonLoadFile(EOH_AIB_LIVE_FILE, live);

            if (!live)
            {
                Print("[EoH_AIBGenerator] Existing DynamicAIB.json failed to load. Aborting merge to protect live config.");
                return;
            }

            JsonFileLoader<EoH_AIBDynamicConfig>.JsonSaveFile(EOH_AIB_BACKUP_FILE, live);
            Print("[EoH_AIBGenerator] Backed up live DynamicAIB before merge: " + EOH_AIB_BACKUP_FILE);
        }
        else
        {
            Print("[EoH_AIBGenerator] Live DynamicAIB.json not found. Creating new merged file at: " + EOH_AIB_LIVE_FILE);
        }

        EnsureConfigValid(live);
        RemoveOldEoHGeneratedEntries(live);
        MergeGeneratedEntries(live, generated);

        JsonFileLoader<EoH_AIBDynamicConfig>.JsonSaveFile(EOH_AIB_LIVE_FILE, live);
        Print("[EoH_AIBGenerator] Merged EoH AI into live DynamicAIB.json: " + EOH_AIB_LIVE_FILE);
    }

    protected static EoH_AIBDynamicConfig BuildGeneratedConfig(EoH_WorldStateData state)
    {
        if (!state || !state.Towns)
        {
            Print("[EoH_AIBGenerator] No world state available. Skipping generation.");
            return null;
        }

        MakeDirectory("$profile:EoH");
        MakeDirectory("$profile:EoH\\WorldState");
        MakeDirectory("$profile:AI_Bandits");

        EoH_AIBDynamicConfig cfg = new EoH_AIBDynamicConfig();
        AddDefaultWeapons(cfg);

        foreach (EoH_WorldStateTownState town : state.Towns)
        {
            if (!town)
                continue;

            if (town.OwnerGroupID == "" || town.OwnerGroupName == "Unclaimed")
                continue;

            AddTownGroup(cfg, town);

            if (town.Tier >= 3)
                AddTownSniper(cfg, town);
        }

        return cfg;
    }

    protected static void EnsureConfigValid(EoH_AIBDynamicConfig cfg)
    {
        if (!cfg.GroupLocations)
            cfg.GroupLocations = new array<ref EoH_AIBGroupLocation>();
        if (!cfg.SniperLocations)
            cfg.SniperLocations = new array<ref EoH_AIBSniperLocation>();
        if (!cfg.PredefinedWeapons)
            cfg.PredefinedWeapons = new array<ref EoH_AIBPredefinedWeapon>();
        if (cfg.version <= 0)
            cfg.version = 2;
        if (cfg.crashsitegroup == "")
            cfg.crashsitegroup = "Random";
    }

    protected static void RemoveOldEoHGeneratedEntries(EoH_AIBDynamicConfig cfg)
    {
        for (int i = cfg.GroupLocations.Count() - 1; i >= 0; i--)
        {
            EoH_AIBGroupLocation group = cfg.GroupLocations.Get(i);
            if (group && IsEoHGeneratedName(group.name))
                cfg.GroupLocations.Remove(i);
        }

        for (int s = cfg.SniperLocations.Count() - 1; s >= 0; s--)
        {
            EoH_AIBSniperLocation sniper = cfg.SniperLocations.Get(s);
            if (sniper && IsEoHGeneratedName(sniper.name))
                cfg.SniperLocations.Remove(s);
        }

        for (int w = cfg.PredefinedWeapons.Count() - 1; w >= 0; w--)
        {
            EoH_AIBPredefinedWeapon weapon = cfg.PredefinedWeapons.Get(w);
            if (weapon && IsEoHGeneratedName(weapon.name))
                cfg.PredefinedWeapons.Remove(w);
        }
    }

    protected static bool IsEoHGeneratedName(string name)
    {
        if (name.Length() < 4)
            return false;
        return name.Substring(0, 4) == "EoH_";
    }

    protected static void MergeGeneratedEntries(EoH_AIBDynamicConfig live, EoH_AIBDynamicConfig generated)
    {
        foreach (EoH_AIBPredefinedWeapon weapon : generated.PredefinedWeapons)
        {
            if (weapon)
                live.PredefinedWeapons.Insert(weapon);
        }
        foreach (EoH_AIBGroupLocation group : generated.GroupLocations)
        {
            if (group)
                live.GroupLocations.Insert(group);
        }
        foreach (EoH_AIBSniperLocation sniper : generated.SniperLocations)
        {
            if (sniper)
                live.SniperLocations.Insert(sniper);
        }
    }

    protected static void AddTownGroup(EoH_AIBDynamicConfig cfg, EoH_WorldStateTownState town)
    {
        EoH_AIBGroupLocation group = new EoH_AIBGroupLocation();
        group.name = "EoH_" + town.Name + "_T" + town.Tier.ToString();
        group.faction = "Bandits";
        group.accuracy = GetAccuracyForTier(town.Tier);
        group.grenadechance = GetGrenadeChanceForTier(town.Tier);
        group.dog = GetDogForTier(town.Tier);

        AddNPCClassesForTier(group.npcclasses, town.Tier);
        AddWeaponsForTier(group.weaponpool, town.Tier);
        AddNPCPropertiesForTier(group.npcproperties, town.Tier);
        AddWaypointsForTown(group.waypoints, town.Name, town.Tier);

        cfg.GroupLocations.Insert(group);
    }

    protected static void AddTownSniper(EoH_AIBDynamicConfig cfg, EoH_WorldStateTownState town)
    {
        EoH_AIBSniperLocation sniper = new EoH_AIBSniperLocation();
        sniper.name = "EoH_" + town.Name + "_Overwatch";
        sniper.npcclass = "BanditAI_Keiko";
        sniper.accuracy = GetAccuracyForTier(town.Tier) + 5.0;
        sniper.fixedpos = 0;

        AddSniperPositionsForTown(sniper.positions, town.Name, town.Tier);

        if (sniper.positions.Count() > 0)
            sniper.triggerpos = sniper.positions.Get(0);

        sniper.weaponpool.Insert("EoH_Sniper_Mosin");
        AddNPCPropertiesForTier(sniper.npcproperties, town.Tier);

        cfg.SniperLocations.Insert(sniper);
    }

    protected static void AddDefaultWeapons(EoH_AIBDynamicConfig cfg)
    {
        TStringArray akmAttachments = new TStringArray;
        akmAttachments.Insert("Mag_AKM_30Rnd");
        akmAttachments.Insert("AK_WoodHndgrd");
        akmAttachments.Insert("AK_WoodBttstck");
        AddWeapon(cfg, "EoH_Low_AKM", "AKM", akmAttachments);

        TStringArray m4Attachments = new TStringArray;
        m4Attachments.Insert("Mag_STANAG_30Rnd");
        m4Attachments.Insert("M4_PlasticHndgrd");
        m4Attachments.Insert("M4_OEBttstck");
        AddWeapon(cfg, "EoH_Mid_M4", "M4A1", m4Attachments);

        TStringArray ak74Attachments = new TStringArray;
        ak74Attachments.Insert("Mag_AK74_30Rnd");
        ak74Attachments.Insert("AK74_Hndgrd");
        ak74Attachments.Insert("AK74_WoodBttstck");
        AddWeapon(cfg, "EoH_High_AK74", "AK74", ak74Attachments);

        TStringArray mosinAttachments = new TStringArray;
        mosinAttachments.Insert("Ammo_762x54");
        AddWeapon(cfg, "EoH_Sniper_Mosin", "Mosin9130", mosinAttachments);
    }

    protected static void AddWeapon(EoH_AIBDynamicConfig cfg, string name, string weapon, TStringArray attachments)
    {
        EoH_AIBPredefinedWeapon w = new EoH_AIBPredefinedWeapon();
        w.name = name;
        w.weapon = weapon;
        foreach (string item : attachments)
            w.attachments.Insert(item);
        cfg.PredefinedWeapons.Insert(w);
    }

    protected static float GetAccuracyForTier(int tier)
    {
        if (tier <= 1) return 55.0;
        if (tier == 2) return 65.0;
        if (tier == 3) return 75.0;
        return 85.0;
    }

    protected static float GetGrenadeChanceForTier(int tier)
    {
        if (tier <= 1) return 0.0;
        if (tier == 2) return 5.0;
        if (tier == 3) return 10.0;
        return 20.0;
    }

    protected static int GetDogForTier(int tier)
    {
        if (tier <= 2) return 0;
        return 1;
    }

    protected static int GetAIGroupCountForTier(int tier)
    {
        if (tier <= 1) return 3;   // small towns: light defensive team
        if (tier == 2) return 5;   // mid towns: proper patrol group
        if (tier == 3) return 7;   // cities: outward pressure group
        return 10;                 // military: assault-level squad
    }

    protected static void AddNPCClassesForTier(array<string> npcclasses, int tier)
    {
        TStringArray pool = new TStringArray;
        pool.Insert("BanditAI_Keiko");
        pool.Insert("BanditAI_Linda");
        pool.Insert("BanditAI_Rolf");
        pool.Insert("BanditAI_Denis");
        pool.Insert("BanditAI_Mirek");

        int count = GetAIGroupCountForTier(tier);

        for (int i = 0; i < count; i++)
        {
            int index = i % pool.Count();
            npcclasses.Insert(pool.Get(index));
        }
    }

    protected static void AddWeaponsForTier(array<string> weaponpool, int tier)
    {
        if (tier <= 1)
        {
            weaponpool.Insert("EoH_Low_AKM");
            return;
        }
        if (tier == 2)
        {
            weaponpool.Insert("EoH_Low_AKM");
            weaponpool.Insert("EoH_Mid_M4");
            return;
        }
        if (tier == 3)
        {
            weaponpool.Insert("EoH_Mid_M4");
            weaponpool.Insert("EoH_High_AK74");
            return;
        }
        weaponpool.Insert("EoH_Mid_M4");
        weaponpool.Insert("EoH_High_AK74");
        weaponpool.Insert("EoH_Sniper_Mosin");
    }

    protected static void AddNPCPropertiesForTier(EoH_AIBNPCProperties props, int tier)
    {
        if (tier <= 1)
        {
            props.backpacks.Insert("HuntingBag");
            props.bodywear.Insert("HuntingJacket_Brown");
            props.pants.Insert("HunterPants_Brown");
            props.shoes.Insert("CombatBoots_Black");
            props.loot.Insert("Rag");
            props.loot.Insert("Apple");
            return;
        }

        props.vests.Insert("PlateCarrierVest");
        props.backpacks.Insert("TacticalBag_Black");
        props.bodywear.Insert("BDUJacket");
        props.pants.Insert("BDUPants");
        props.shoes.Insert("CombatBoots_Black");
        props.gloves.Insert("TacticalGloves_Black");
        props.loot.Insert("Rag");
        props.loot.Insert("BandageDressing");

        if (tier >= 3)
        {
            props.headgear.Insert("Mich2001Helmet");
            props.masks.Insert("BalaclavaMask_Black");
            props.loot.Insert("Morphine");
        }
    }

    protected static void AddWaypointsForTown(array<string> waypoints, string townName, int tier)
    {
        if (townName == "Vybor")
        {
            waypoints.Insert("3700 0 8800");
            waypoints.Insert("3500 0 8700");
            waypoints.Insert("3900 0 9000");
            return;
        }
        if (townName == "Stary Sobor")
        {
            waypoints.Insert("6200 0 7800");
            waypoints.Insert("6000 0 7700");
            waypoints.Insert("6400 0 8000");
            return;
        }
        if (townName == "Elektro")
        {
            waypoints.Insert("10500 0 2250");
            waypoints.Insert("11000 0 2500");
            waypoints.Insert("9800 0 2400");
            return;
        }
        if (townName == "NWAF")
        {
            waypoints.Insert("4800 0 9600");
            waypoints.Insert("4500 0 9400");
            waypoints.Insert("5200 0 9800");
            waypoints.Insert("5000 0 10200");
            return;
        }
        waypoints.Insert("0 0 0");
    }

    protected static void AddSniperPositionsForTown(array<string> positions, string townName, int tier)
    {
        if (townName == "Elektro")
        {
            positions.Insert("10200 0 2000");
            positions.Insert("11000 0 2500");
            return;
        }
        if (townName == "NWAF")
        {
            positions.Insert("5000 0 10200");
            positions.Insert("4700 0 9200");
            return;
        }
        positions.Insert("0 0 0");
    }
};
