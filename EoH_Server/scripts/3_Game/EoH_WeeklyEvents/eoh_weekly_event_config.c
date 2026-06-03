class EoH_WeeklyEventConfig
{
    int ConfigVersion = 4;
    bool Enabled = true;

    float AltarRepairDurationSeconds = 60.0;
    float AltarRepairProximityRadius = 5.0;
    float AltarRepairMaintainRadius = 75.0;
    int AltarRepairWatcherIntervalMs = 1000;
    bool EnableAltarProximityAutoRepair = false;
    bool StartAltarAIOnRepairStart = true;
    int AltarAIRounds = 5;
    int AltarRewardWave = 5;
    int AltarWave2DelaySeconds = 240;
    int AltarWave3DelaySeconds = 480;
    int AltarWave4DelaySeconds = 720;
    int AltarWave5DelaySeconds = 960;
    bool ResetAltarRepairWhenGroupLeavesZone = true;
    bool ConsumeAltarRepairItemsAtCompletion = true;
    bool EnableRecoveryGracePeriod = true;
    int RecoveryGraceSeconds = 300;

    void Defaults()
    {
        ConfigVersion = 4;
        Enabled = true;

        AltarRepairDurationSeconds = 60.0;
        AltarRepairProximityRadius = 5.0;
        AltarRepairMaintainRadius = 75.0;
        AltarRepairWatcherIntervalMs = 1000;
        EnableAltarProximityAutoRepair = false;
        StartAltarAIOnRepairStart = true;
        AltarAIRounds = 5;
        AltarRewardWave = 5;
        AltarWave2DelaySeconds = 240;
        AltarWave3DelaySeconds = 480;
        AltarWave4DelaySeconds = 720;
        AltarWave5DelaySeconds = 960;
        ResetAltarRepairWhenGroupLeavesZone = true;
        ConsumeAltarRepairItemsAtCompletion = true;
        EnableRecoveryGracePeriod = true;
        RecoveryGraceSeconds = 300;
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
            Print("[EoH_WeeklyEventConfig] Loaded config repairSeconds=" + m_Config.AltarRepairDurationSeconds.ToString() + " startRadius=" + m_Config.AltarRepairProximityRadius.ToString() + " maintainRadius=" + m_Config.AltarRepairMaintainRadius.ToString() + " autoRepair=" + m_Config.EnableAltarProximityAutoRepair.ToString() + " aiRounds=" + m_Config.AltarAIRounds.ToString() + " rewardWave=" + m_Config.AltarRewardWave.ToString() + " wave2=" + m_Config.AltarWave2DelaySeconds.ToString() + " wave3=" + m_Config.AltarWave3DelaySeconds.ToString() + " wave4=" + m_Config.AltarWave4DelaySeconds.ToString() + " wave5=" + m_Config.AltarWave5DelaySeconds.ToString() + " recoveryGrace=" + m_Config.EnableRecoveryGracePeriod.ToString() + " graceSeconds=" + m_Config.RecoveryGraceSeconds.ToString());
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

        if (m_Config.ConfigVersion < 4)
            m_Config.ConfigVersion = 4;

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

        if (m_Config.AltarWave2DelaySeconds <= 0)
            m_Config.AltarWave2DelaySeconds = 240;

        if (m_Config.AltarWave3DelaySeconds <= 0)
            m_Config.AltarWave3DelaySeconds = 480;

        if (m_Config.AltarWave4DelaySeconds <= 0)
            m_Config.AltarWave4DelaySeconds = 720;

        if (m_Config.AltarWave5DelaySeconds <= 0)
            m_Config.AltarWave5DelaySeconds = 960;

        if (m_Config.RecoveryGraceSeconds <= 0)
            m_Config.RecoveryGraceSeconds = 300;
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

    int GetAltarWaveDelaySeconds(int waveIndex)
    {
        if (!m_Config)
        {
            if (waveIndex == 2)
                return 240;
            if (waveIndex == 3)
                return 480;
            if (waveIndex == 4)
                return 720;
            if (waveIndex >= 5)
                return 960;
            return 0;
        }

        if (waveIndex == 2)
            return m_Config.AltarWave2DelaySeconds;
        if (waveIndex == 3)
            return m_Config.AltarWave3DelaySeconds;
        if (waveIndex == 4)
            return m_Config.AltarWave4DelaySeconds;
        if (waveIndex >= 5)
            return m_Config.AltarWave5DelaySeconds;

        return 0;
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

    bool IsRecoveryGracePeriodEnabled()
    {
        if (!m_Config)
            return true;
        return m_Config.EnableRecoveryGracePeriod;
    }

    int GetRecoveryGraceSeconds()
    {
        if (!m_Config || m_Config.RecoveryGraceSeconds <= 0)
            return 300;
        return m_Config.RecoveryGraceSeconds;
    }
}
