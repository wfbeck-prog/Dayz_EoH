class EoH_QuestTravelOverlayFileLoader
{
    protected static ref array<ref EoH_QuestTravelOverlayData> s_Cache;
    protected const static string CONFIG_DIR = "$profile:EoH";
    protected const static string CONFIG_PATH = "$profile:EoH/QuestTravelOverlayConfig.json";

    static array<ref EoH_QuestTravelOverlayData> Get()
    {
        if (!s_Cache)
            Load();

        return s_Cache;
    }

    static void Load()
    {
        s_Cache = new array<ref EoH_QuestTravelOverlayData>();

        EnsureConfigDir();

        EoH_QuestTravelOverlayFileConfig cfg = new EoH_QuestTravelOverlayFileConfig();

        if (!FileExist(CONFIG_PATH))
        {
            SeedDefaults(cfg);
            JsonFileLoader<EoH_QuestTravelOverlayFileConfig>.JsonSaveFile(CONFIG_PATH, cfg);
            Print("[EoH_QuestOverlayLoader] Created default overlay config file path=" + CONFIG_PATH);
        }
        else
        {
            JsonFileLoader<EoH_QuestTravelOverlayFileConfig>.JsonLoadFile(CONFIG_PATH, cfg);
            Print("[EoH_QuestOverlayLoader] Loaded overlay config file path=" + CONFIG_PATH + " entries=" + cfg.Overlays.Count().ToString());
        }

        foreach (EoH_QuestTravelOverlayFileEntry entry : cfg.Overlays)
        {
            if (!entry)
                continue;

            // Config stores the REAL objective position as a safe DayZ vector string.
            // Overlay system automatically offsets the visible circle center.
            vector truePos = entry.TruePosition.ToVector();
            EoH_QuestTravelOverlayData overlay = new EoH_QuestTravelOverlayData(entry.QuestID, entry.ObjectiveID, entry.Label, truePos, entry.Radius);
            s_Cache.Insert(overlay);
        }

        Print("[EoH_QuestOverlayLoader] Active overlay entries=" + s_Cache.Count().ToString());
    }

    static void EnsureConfigDir()
    {
        if (!FileExist(CONFIG_DIR))
        {
            MakeDirectory(CONFIG_DIR);
            Print("[EoH_QuestOverlayLoader] Created config directory path=" + CONFIG_DIR);
        }
    }

    static void Reload()
    {
        s_Cache = null;
        Load();
    }

    static void SeedDefaults(EoH_QuestTravelOverlayFileConfig cfg)
    {
        if (!cfg)
            return;

        ref EoH_QuestTravelOverlayFileEntry example = new EoH_QuestTravelOverlayFileEntry();
        example.QuestID = 401001;
        example.ObjectiveID = 501001;
        example.Label = "Signal and Ashes: Search Area";

        // REAL objective location.
        // Visible search circle center is automatically offset.
        example.TruePosition = "3060 310 7870";

        example.Radius = 350.0;

        cfg.Overlays.Insert(example);
    }
};
