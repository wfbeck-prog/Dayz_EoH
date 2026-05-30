class EoH_WeeklyEventConfig
{
    int ConfigVersion = 1;
    bool Enabled = true;

    float AltarRepairDurationSeconds = 10.0;
    float AltarRepairProximityRadius = 5.0;
    int AltarRepairWatcherIntervalMs = 5000;
    bool EnableAltarProximityAutoRepair = false;

    void Defaults()
    {
        ConfigVersion = 1;
        Enabled = true;

        AltarRepairDurationSeconds = 10.0;
        AltarRepairProximityRadius = 5.0;
        AltarRepairWatcherIntervalMs = 5000;
        EnableAltarProximityAutoRepair = false;
    }
}

class EoH_WeeklyEventConfigManager
{
    protected static ref EoH_WeeklyEventConfigManager s_Instance;
    protected ref EoH_WeeklyEventConfig m_Config;

    protected const string CONFIG_DIR = "$profile:EoH";
    protected const string CONFIG_PATH = "$profile:EoH/WeeklyEventConfig.json";

    void EoH_WeeklyEventConfigManager()
    {
        LoadConfig();
    }

    static EoH_WeeklyEventConfigManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_WeeklyEventConfigManager();

        return s_Instance;
    }

    void LoadConfig()
    {
        EnsureConfigDir();
        m_Config = new EoH_WeeklyEventConfig();

        if (!FileExist(CONFIG_PATH))
        {
            m_Config.Defaults();
            JsonFileLoader<EoH_WeeklyEventConfig>.JsonSaveFile(CONFIG_PATH, m_Config);
            Print("[EoH_WeeklyEventConfig] Created default config path=" + CONFIG_PATH);
        }
        else
        {
            JsonFileLoader<EoH_WeeklyEventConfig>.JsonLoadFile(CONFIG_PATH, m_Config);
            NormalizeConfig();
            Print("[EoH_WeeklyEventConfig] Loaded config repairSeconds=" + m_Config.AltarRepairDurationSeconds.ToString() + " proximityRadius=" + m_Config.AltarRepairProximityRadius.ToString() + " autoRepair=" + m_Config.EnableAltarProximityAutoRepair.ToString());
        }
    }

    void NormalizeConfig()
    {
        if (!m_Config)
        {
            m_Config = new EoH_WeeklyEventConfig();
            m_Config.Defaults();
            return;
        }

        if (m_Config.AltarRepairDurationSeconds <= 0)
            m_Config.AltarRepairDurationSeconds = 10.0;

        if (m_Config.AltarRepairProximityRadius <= 0)
            m_Config.AltarRepairProximityRadius = 5.0;

        if (m_Config.AltarRepairWatcherIntervalMs <= 0)
            m_Config.AltarRepairWatcherIntervalMs = 5000;
    }

    void EnsureConfigDir()
    {
        if (!FileExist(CONFIG_DIR))
            MakeDirectory(CONFIG_DIR);
    }

    float GetAltarRepairDurationSeconds()
    {
        if (!m_Config)
            return 10.0;

        if (m_Config.AltarRepairDurationSeconds <= 0)
            return 10.0;

        return m_Config.AltarRepairDurationSeconds;
    }

    float GetAltarRepairProximityRadius()
    {
        if (!m_Config)
            return 5.0;

        if (m_Config.AltarRepairProximityRadius <= 0)
            return 5.0;

        return m_Config.AltarRepairProximityRadius;
    }

    int GetAltarRepairWatcherIntervalMs()
    {
        if (!m_Config)
            return 5000;

        if (m_Config.AltarRepairWatcherIntervalMs <= 0)
            return 5000;

        return m_Config.AltarRepairWatcherIntervalMs;
    }

    bool IsAltarProximityAutoRepairEnabled()
    {
        if (!m_Config)
            return false;

        return m_Config.EnableAltarProximityAutoRepair;
    }
}
