class EoH_WeekendIntelConfig
{
    int ConfigVersion = 1;
    bool Enabled = false;

    bool AllowTownCaptureIntelRewards = false;
    bool AllowQuestBoardIntelRewards = false;
    bool AllowBlackMarketIntelSales = false;
    bool AllowAIDrops = false;

    float TownCaptureIntelChance = 0.15;
    float QuestBoardIntelChance = 0.25;
    float AIDropIntelChance = 0.05;

    string WeekendIntelClassName = "EoH_WeekendEventIntel";
    int TownCaptureIntelMin = 1;
    int TownCaptureIntelMax = 1;

    void Defaults()
    {
        ConfigVersion = 1;
        Enabled = false;

        AllowTownCaptureIntelRewards = false;
        AllowQuestBoardIntelRewards = false;
        AllowBlackMarketIntelSales = false;
        AllowAIDrops = false;

        TownCaptureIntelChance = 0.15;
        QuestBoardIntelChance = 0.25;
        AIDropIntelChance = 0.05;

        WeekendIntelClassName = "EoH_WeekendEventIntel";
        TownCaptureIntelMin = 1;
        TownCaptureIntelMax = 1;
    }
}

class EoH_WeekendIntelConfigManager
{
    protected static ref EoH_WeekendIntelConfigManager s_Instance;
    protected ref EoH_WeekendIntelConfig m_Config;

    protected const string CONFIG_DIR = "$profile:EoH";
    protected const string CONFIG_PATH = "$profile:EoH/WeekendIntelConfig.json";

    void EoH_WeekendIntelConfigManager()
    {
        LoadConfig();
    }

    static EoH_WeekendIntelConfigManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_WeekendIntelConfigManager();

        return s_Instance;
    }

    void LoadConfig()
    {
        EnsureConfigDir();
        m_Config = new EoH_WeekendIntelConfig();

        if (!FileExist(CONFIG_PATH))
        {
            m_Config.Defaults();
            JsonFileLoader<EoH_WeekendIntelConfig>.JsonSaveFile(CONFIG_PATH, m_Config);
            Print("[EoH_WeekendIntelConfig] Created default config path=" + CONFIG_PATH);
        }
        else
        {
            JsonFileLoader<EoH_WeekendIntelConfig>.JsonLoadFile(CONFIG_PATH, m_Config);
            NormalizeConfig();
            Print("[EoH_WeekendIntelConfig] Loaded enabled=" + m_Config.Enabled.ToString() + " townCapture=" + m_Config.AllowTownCaptureIntelRewards.ToString() + " intelClass=" + m_Config.WeekendIntelClassName);
        }
    }

    void NormalizeConfig()
    {
        if (!m_Config)
        {
            m_Config = new EoH_WeekendIntelConfig();
            m_Config.Defaults();
            return;
        }

        if (m_Config.ConfigVersion < 1)
            m_Config.ConfigVersion = 1;

        if (m_Config.WeekendIntelClassName == "")
            m_Config.WeekendIntelClassName = "EoH_WeekendEventIntel";

        if (m_Config.TownCaptureIntelChance < 0.0)
            m_Config.TownCaptureIntelChance = 0.0;
        if (m_Config.TownCaptureIntelChance > 1.0)
            m_Config.TownCaptureIntelChance = 1.0;

        if (m_Config.QuestBoardIntelChance < 0.0)
            m_Config.QuestBoardIntelChance = 0.0;
        if (m_Config.QuestBoardIntelChance > 1.0)
            m_Config.QuestBoardIntelChance = 1.0;

        if (m_Config.AIDropIntelChance < 0.0)
            m_Config.AIDropIntelChance = 0.0;
        if (m_Config.AIDropIntelChance > 1.0)
            m_Config.AIDropIntelChance = 1.0;

        if (m_Config.TownCaptureIntelMin < 0)
            m_Config.TownCaptureIntelMin = 0;
        if (m_Config.TownCaptureIntelMax < m_Config.TownCaptureIntelMin)
            m_Config.TownCaptureIntelMax = m_Config.TownCaptureIntelMin;
    }

    void EnsureConfigDir()
    {
        if (!FileExist(CONFIG_DIR))
            MakeDirectory(CONFIG_DIR);
    }

    bool IsEnabled()
    {
        if (!m_Config)
            return false;
        return m_Config.Enabled;
    }

    bool CanAwardTownCaptureIntel()
    {
        if (!m_Config || !m_Config.Enabled)
            return false;
        return m_Config.AllowTownCaptureIntelRewards;
    }

    bool CanAwardQuestBoardIntel()
    {
        if (!m_Config || !m_Config.Enabled)
            return false;
        return m_Config.AllowQuestBoardIntelRewards;
    }

    bool CanSellBlackMarketIntel()
    {
        if (!m_Config || !m_Config.Enabled)
            return false;
        return m_Config.AllowBlackMarketIntelSales;
    }

    bool CanDropAIIntel()
    {
        if (!m_Config || !m_Config.Enabled)
            return false;
        return m_Config.AllowAIDrops;
    }

    string GetWeekendIntelClassName()
    {
        if (!m_Config || m_Config.WeekendIntelClassName == "")
            return "EoH_WeekendEventIntel";
        return m_Config.WeekendIntelClassName;
    }

    bool RollTownCaptureIntelReward()
    {
        if (!CanAwardTownCaptureIntel())
            return false;
        return Math.RandomFloat01() <= m_Config.TownCaptureIntelChance;
    }

    bool RollQuestBoardIntelReward()
    {
        if (!CanAwardQuestBoardIntel())
            return false;
        return Math.RandomFloat01() <= m_Config.QuestBoardIntelChance;
    }

    bool RollAIDropIntelReward()
    {
        if (!CanDropAIIntel())
            return false;
        return Math.RandomFloat01() <= m_Config.AIDropIntelChance;
    }

    int GetTownCaptureIntelCount()
    {
        if (!m_Config)
            return 0;

        if (m_Config.TownCaptureIntelMax <= 0)
            return 0;

        return Math.RandomIntInclusive(m_Config.TownCaptureIntelMin, m_Config.TownCaptureIntelMax);
    }
}
