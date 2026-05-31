class EoH_WeeklyEventRewardItem
{
    string ClassName;
    int Min;
    int Max;
    float Chance;

    void EoH_WeeklyEventRewardItem()
    {
        ClassName = "";
        Min = 1;
        Max = 1;
        Chance = 1.0;
    }
}

class EoH_WeeklyEventRewardConfig
{
    int ConfigVersion = 1;
    ref array<ref EoH_WeeklyEventRewardItem> AltarRewardItems;
    ref array<ref EoH_WeeklyEventRewardItem> PurgeNightRewardItems;

    void EoH_WeeklyEventRewardConfig()
    {
        AltarRewardItems = new array<ref EoH_WeeklyEventRewardItem>();
        PurgeNightRewardItems = new array<ref EoH_WeeklyEventRewardItem>();
    }

    void Defaults()
    {
        ConfigVersion = 1;

        if (!AltarRewardItems)
            AltarRewardItems = new array<ref EoH_WeeklyEventRewardItem>();
        if (!PurgeNightRewardItems)
            PurgeNightRewardItems = new array<ref EoH_WeeklyEventRewardItem>();

        AltarRewardItems.Clear();
        PurgeNightRewardItems.Clear();

        AddAltarReward("PunchedCard", 1, 1, 1.0);
        AddAltarReward("DNA_Keycard_Red", 1, 1, 1.0);
        AddAltarReward("EoH_TownIntel", 1, 2, 0.75);
        AddAltarReward("EoH_TraderIntel", 1, 1, 0.50);
        AddAltarReward("Money_Dollar100", 5, 15, 0.85);

        AddPurgeReward("PunchedCard", 1, 1, 0.75);
        AddPurgeReward("DNA_Keycard_Red", 1, 1, 0.75);
        AddPurgeReward("EoH_TownIntel", 1, 2, 0.65);
        AddPurgeReward("EoH_TraderIntel", 1, 1, 0.50);
        AddPurgeReward("Money_Dollar100", 5, 20, 0.85);
    }

    void AddAltarReward(string className, int minCount, int maxCount, float chance)
    {
        EoH_WeeklyEventRewardItem item = new EoH_WeeklyEventRewardItem();
        item.ClassName = className;
        item.Min = minCount;
        item.Max = maxCount;
        item.Chance = chance;
        AltarRewardItems.Insert(item);
    }

    void AddPurgeReward(string className, int minCount, int maxCount, float chance)
    {
        EoH_WeeklyEventRewardItem item = new EoH_WeeklyEventRewardItem();
        item.ClassName = className;
        item.Min = minCount;
        item.Max = maxCount;
        item.Chance = chance;
        PurgeNightRewardItems.Insert(item);
    }
}

class EoH_WeeklyEventRewardConfigManager
{
    protected static ref EoH_WeeklyEventRewardConfigManager s_Instance;
    protected ref EoH_WeeklyEventRewardConfig m_Config;

    protected const string CONFIG_DIR = "$profile:EoH";
    protected const string CONFIG_PATH = "$profile:EoH/WeeklyEventRewards.json";

    void EoH_WeeklyEventRewardConfigManager()
    {
        LoadConfig();
    }

    static EoH_WeeklyEventRewardConfigManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_WeeklyEventRewardConfigManager();

        return s_Instance;
    }

    void LoadConfig()
    {
        EnsureConfigDir();
        m_Config = new EoH_WeeklyEventRewardConfig();

        if (!FileExist(CONFIG_PATH))
        {
            m_Config.Defaults();
            JsonFileLoader<EoH_WeeklyEventRewardConfig>.JsonSaveFile(CONFIG_PATH, m_Config);
            Print("[EoH_WeeklyEventRewards] Created default config path=" + CONFIG_PATH);
        }
        else
        {
            JsonFileLoader<EoH_WeeklyEventRewardConfig>.JsonLoadFile(CONFIG_PATH, m_Config);
            NormalizeConfig();
            Print("[EoH_WeeklyEventRewards] Loaded config altarItems=" + m_Config.AltarRewardItems.Count().ToString() + " purgeItems=" + m_Config.PurgeNightRewardItems.Count().ToString());
        }
    }

    void NormalizeConfig()
    {
        if (!m_Config)
        {
            m_Config = new EoH_WeeklyEventRewardConfig();
            m_Config.Defaults();
            return;
        }

        if (!m_Config.AltarRewardItems)
            m_Config.AltarRewardItems = new array<ref EoH_WeeklyEventRewardItem>();
        if (!m_Config.PurgeNightRewardItems)
            m_Config.PurgeNightRewardItems = new array<ref EoH_WeeklyEventRewardItem>();

        if (m_Config.ConfigVersion < 1)
            m_Config.ConfigVersion = 1;
    }

    void EnsureConfigDir()
    {
        if (!FileExist(CONFIG_DIR))
            MakeDirectory(CONFIG_DIR);
    }

    array<ref EoH_WeeklyEventRewardItem> GetRewardItems(string eventId)
    {
        if (!m_Config)
        {
            m_Config = new EoH_WeeklyEventRewardConfig();
            m_Config.Defaults();
        }

        if (eventId == "altar_relay_towers")
            return m_Config.AltarRewardItems;

        if (eventId == "purge_night_novy_stary")
            return m_Config.PurgeNightRewardItems;

        return m_Config.AltarRewardItems;
    }

    void FillRewardCrate(EntityAI crate, string eventId)
    {
        if (!crate)
            return;

        array<ref EoH_WeeklyEventRewardItem> items = GetRewardItems(eventId);
        if (!items || items.Count() == 0)
        {
            Print("[EoH_WeeklyEventRewards][WARN] Empty reward pool eventId=" + eventId);
            return;
        }

        int added = 0;
        foreach (EoH_WeeklyEventRewardItem reward : items)
        {
            if (!reward || reward.ClassName == "")
                continue;

            float chance = reward.Chance;
            if (chance < 0.0)
                chance = 0.0;
            if (chance > 1.0)
                chance = 1.0;

            if (Math.RandomFloat01() > chance)
                continue;

            int minCount = reward.Min;
            int maxCount = reward.Max;
            if (minCount < 1)
                minCount = 1;
            if (maxCount < minCount)
                maxCount = minCount;

            int count = Math.RandomIntInclusive(minCount, maxCount);
            for (int i = 0; i < count; i++)
            {
                EntityAI item = crate.GetInventory().CreateInInventory(reward.ClassName);
                if (item)
                    added++;
                else
                    Print("[EoH_WeeklyEventRewards][WARN] Failed to add reward item=" + reward.ClassName + " eventId=" + eventId);
            }
        }

        Print("[EoH_WeeklyEventRewards] Filled crate eventId=" + eventId + " added=" + added.ToString());
    }
}
