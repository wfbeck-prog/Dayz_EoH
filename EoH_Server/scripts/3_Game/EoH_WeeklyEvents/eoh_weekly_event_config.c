class EoH_WeeklyEventConfig
{
    int ConfigVersion = 2;
    bool Enabled = true;

    float AltarRepairDurationSeconds = 60.0;
    float AltarRepairProximityRadius = 5.0;
    float AltarRepairMaintainRadius = 75.0;
    int AltarRepairWatcherIntervalMs = 1000;
    bool EnableAltarProximityAutoRepair = false;
    bool StartAltarAIOnRepairStart = true;
    int AltarAIRounds = 5;
    int AltarRewardWave = 5;
    bool ResetAltarRepairWhenGroupLeavesZone = true;
    bool ConsumeAltarRepairItemsAtCompletion = true;

    void Defaults()
    {
        ConfigVersion = 2;
        Enabled = true;

        AltarRepairDurationSeconds = 60.0;
        AltarRepairProximityRadius = 5.0;
        AltarRepairMaintainRadius = 75.0;
        AltarRepairWatcherIntervalMs = 1000;
        EnableAltarProximityAutoRepair = false;
        StartAltarAIOnRepairStart = true;
        AltarAIRounds = 5;
        AltarRewardWave = 5;
        ResetAltarRepairWhenGroupLeavesZone = true;
        ConsumeAltarRepairItemsAtCompletion = true;
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
            Print("[EoH_WeeklyEventConfig] Loaded config repairSeconds=" + m_Config.AltarRepairDurationSeconds.ToString() + " startRadius=" + m_Config.AltarRepairProximityRadius.ToString() + " maintainRadius=" + m_Config.AltarRepairMaintainRadius.ToString() + " autoRepair=" + m_Config.EnableAltarProximityAutoRepair.ToString() + " aiRounds=" + m_Config.AltarAIRounds.ToString() + " rewardWave=" + m_Config.AltarRewardWave.ToString());
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

        if (m_Config.ConfigVersion < 2)
            m_Config.ConfigVersion = 2;

        if (m_Config.AltarRepairDurationSeconds <= 0)
            m_Config.AltarRepairDurationSeconds = 60.0;

        if (m_Config.AltarRepairProximityRadius <= 0)
            m_Config.AltarRepairProximityRadius = 5.0;

        if (m_Config.AltarRepairMaintainRadius <= 0)
            m_Config.AltarRepairMaintainRadius = 75.0;

        if (m_Config.AltarRepairWatcherIntervalMs <= 0)
            m_Config.AltarRepairWatcherIntervalMs = 1000;

        if (m_Config.AltarAIRounds <= 0)
            m_Config.AltarAIRounds = 5;

        if (m_Config.AltarRewardWave <= 0)
            m_Config.AltarRewardWave = 5;
    }

    void EnsureConfigDir()
    {
        if (!FileExist(CONFIG_DIR))
            MakeDirectory(CONFIG_DIR);
    }

    float GetAltarRepairDurationSeconds()
    {
        if (!m_Config || m_Config.AltarRepairDurationSeconds <= 0)
            return 60.0;
        return m_Config.AltarRepairDurationSeconds;
    }

    float GetAltarRepairProximityRadius()
    {
        if (!m_Config || m_Config.AltarRepairProximityRadius <= 0)
            return 5.0;
        return m_Config.AltarRepairProximityRadius;
    }

    float GetAltarRepairMaintainRadius()
    {
        if (!m_Config || m_Config.AltarRepairMaintainRadius <= 0)
            return 75.0;
        return m_Config.AltarRepairMaintainRadius;
    }

    int GetAltarRepairWatcherIntervalMs()
    {
        if (!m_Config || m_Config.AltarRepairWatcherIntervalMs <= 0)
            return 1000;
        return m_Config.AltarRepairWatcherIntervalMs;
    }

    bool IsAltarProximityAutoRepairEnabled()
    {
        if (!m_Config)
            return false;
        return m_Config.EnableAltarProximityAutoRepair;
    }

    bool ShouldStartAltarAIOnRepairStart()
    {
        if (!m_Config)
            return true;
        return m_Config.StartAltarAIOnRepairStart;
    }

    int GetAltarAIRounds()
    {
        if (!m_Config || m_Config.AltarAIRounds <= 0)
            return 5;
        return m_Config.AltarAIRounds;
    }

    int GetAltarRewardWave()
    {
        if (!m_Config || m_Config.AltarRewardWave <= 0)
            return 5;
        return m_Config.AltarRewardWave;
    }

    bool ShouldResetAltarRepairWhenGroupLeavesZone()
    {
        if (!m_Config)
            return true;
        return m_Config.ResetAltarRepairWhenGroupLeavesZone;
    }

    bool ShouldConsumeAltarRepairItemsAtCompletion()
    {
        if (!m_Config)
            return true;
        return m_Config.ConsumeAltarRepairItemsAtCompletion;
    }
}
