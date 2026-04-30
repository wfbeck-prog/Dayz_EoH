class EoH_AIBDynamicGenerator
{
    protected static const string EOH_AIB_GENERATED_FILE = "$profile:EoH\\WorldState\\EoH_DynamicAIB_Generated.json";

    static void GenerateFromWorldState(EoH_WorldStateData state)
    {
        if (!state || !state.Towns)
        {
            Print("[EoH_AIBGenerator] No world state available. Skipping generation.");
            return;
        }

        MakeDirectory("$profile:EoH");
        MakeDirectory("$profile:EoH\\WorldState");

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

        JsonFileLoader<EoH_AIBDynamicConfig>.JsonSaveFile(EOH_AIB_GENERATED_FILE, cfg);
        Print("[EoH_AIBGenerator] Generated AI Bandits config: " + EOH_AIB_GENERATED_FILE);
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
        AddWeapon(cfg, "EoH_Low_AKM", "AKM", {"Mag_AKM_30Rnd", "AK_WoodHndgrd", "AK_WoodBttstck"});
        AddWeapon(cfg, "EoH_Mid_M4", "M4A1", {"Mag_STANAG_30Rnd", "M4_PlasticHndgrd", "M4_OEBttstck"});
        AddWeapon(cfg, "EoH_High_AK74", "AK74", {"Mag_AK74_30Rnd", "AK74_Hndgrd", "AK74_WoodBttstck"});
        AddWeapon(cfg, "EoH_Sniper_Mosin", "Mosin9130", {"Ammo_762x54"});
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
        if (tier <= 1) return 0;
        if (tier == 2) return 0;
        return 1;
    }

    protected static void AddNPCClassesForTier(array<string> npcclasses, int tier)
    {
        npcclasses.Insert("BanditAI_Keiko");
        npcclasses.Insert("BanditAI_Linda");

        if (tier >= 2)
            npcclasses.Insert("BanditAI_Rolf");

        if (tier >= 3)
            npcclasses.Insert("BanditAI_Denis");

        if (tier >= 4)
            npcclasses.Insert("BanditAI_Mirek");
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
