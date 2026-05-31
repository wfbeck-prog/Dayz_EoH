class EoH_WeeklyEventPersistenceState
{
    int ConfigVersion = 1;
    bool Enabled = true;

    bool HasActiveObjective = false;
    string ObjectiveId = "";
    string ObjectiveType = "";
    string DisplayName = "";
    vector Position = "0 0 0";

    bool RevealedByIntel = false;
    bool RepairCompleted = false;
    bool Completed = false;
    bool RewardUnlocked = false;

    int StartTime = 0;
    int LastTickTime = 0;
    int CurrentWave = 0;
    int SavedAt = 0;

    string RewardCrateType = "";
    vector RewardPosition = "0 0 0";

    void Defaults()
    {
        ConfigVersion = 1;
        Enabled = true;
        Clear();
    }

    void Clear()
    {
        HasActiveObjective = false;
        ObjectiveId = "";
        ObjectiveType = "";
        DisplayName = "";
        Position = "0 0 0";

        RevealedByIntel = false;
        RepairCompleted = false;
        Completed = false;
        RewardUnlocked = false;

        StartTime = 0;
        LastTickTime = 0;
        CurrentWave = 0;
        SavedAt = 0;

        RewardCrateType = "";
        RewardPosition = "0 0 0";
    }
}

class EoH_WeeklyEventPersistenceManager
{
    protected static ref EoH_WeeklyEventPersistenceManager s_Instance;
    protected ref EoH_WeeklyEventPersistenceState m_State;

    protected const string CONFIG_DIR = "$profile:EoH";
    protected const string STATE_PATH = "$profile:EoH/WeeklyEventRuntimeState.json";

    void EoH_WeeklyEventPersistenceManager()
    {
        LoadState();
    }

    static EoH_WeeklyEventPersistenceManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_WeeklyEventPersistenceManager();

        return s_Instance;
    }

    void EnsureConfigDir()
    {
        if (!FileExist(CONFIG_DIR))
            MakeDirectory(CONFIG_DIR);
    }

    void LoadState()
    {
        EnsureConfigDir();
        m_State = new EoH_WeeklyEventPersistenceState();

        if (!FileExist(STATE_PATH))
        {
            m_State.Defaults();
            JsonFileLoader<EoH_WeeklyEventPersistenceState>.JsonSaveFile(STATE_PATH, m_State);
            EoH_LiveAdvisorActivity.LogActivity("weekly_event", "persistence_created path=" + STATE_PATH);
            return;
        }

        JsonFileLoader<EoH_WeeklyEventPersistenceState>.JsonLoadFile(STATE_PATH, m_State);
        NormalizeState();
        EoH_LiveAdvisorActivity.LogActivity("weekly_event", "persistence_loaded active=" + m_State.HasActiveObjective.ToString() + " objective=" + m_State.ObjectiveId + " repaired=" + m_State.RepairCompleted.ToString() + " wave=" + m_State.CurrentWave.ToString() + " rewardUnlocked=" + m_State.RewardUnlocked.ToString());
    }

    void NormalizeState()
    {
        if (!m_State)
        {
            m_State = new EoH_WeeklyEventPersistenceState();
            m_State.Defaults();
            return;
        }

        if (m_State.ConfigVersion <= 0)
            m_State.ConfigVersion = 1;
    }

    EoH_WeeklyEventPersistenceState GetState()
    {
        return m_State;
    }

    void SaveIdle()
    {
        if (!m_State)
        {
            m_State = new EoH_WeeklyEventPersistenceState();
            m_State.Defaults();
        }

        m_State.Clear();
        m_State.SavedAt = GetGame().GetTime();
        JsonFileLoader<EoH_WeeklyEventPersistenceState>.JsonSaveFile(STATE_PATH, m_State);
        EoH_LiveAdvisorActivity.LogActivity("weekly_event", "persistence_saved_idle");
    }

    void SaveActive(EoH_EventObjectiveRuntime runtime)
    {
        if (!runtime || !runtime.Config)
        {
            SaveIdle();
            return;
        }

        if (!m_State)
            m_State = new EoH_WeeklyEventPersistenceState();

        EoH_EventObjective cfg = runtime.Config;

        m_State.ConfigVersion = 1;
        m_State.Enabled = true;
        m_State.HasActiveObjective = runtime.Active;
        m_State.ObjectiveId = cfg.Id;
        m_State.ObjectiveType = cfg.ObjectiveType;
        m_State.DisplayName = cfg.DisplayName;
        m_State.Position = cfg.Position;

        m_State.RevealedByIntel = runtime.RevealedByIntel;
        m_State.RepairCompleted = runtime.StartTime > 0;
        m_State.Completed = runtime.Completed;
        m_State.RewardUnlocked = runtime.RewardUnlocked;

        m_State.StartTime = runtime.StartTime;
        m_State.LastTickTime = runtime.LastTickTime;
        m_State.CurrentWave = runtime.CurrentWave;
        m_State.SavedAt = GetGame().GetTime();

        if (runtime.RewardCrate)
        {
            m_State.RewardCrateType = runtime.RewardCrate.CrateType;
            m_State.RewardPosition = runtime.RewardCrate.Position;
        }
        else
        {
            m_State.RewardCrateType = "";
            m_State.RewardPosition = "0 0 0";
        }

        JsonFileLoader<EoH_WeeklyEventPersistenceState>.JsonSaveFile(STATE_PATH, m_State);
        EoH_LiveAdvisorActivity.LogActivity("weekly_event", "persistence_saved active=" + m_State.HasActiveObjective.ToString() + " objective=" + m_State.ObjectiveId + " repaired=" + m_State.RepairCompleted.ToString() + " wave=" + m_State.CurrentWave.ToString() + " rewardUnlocked=" + m_State.RewardUnlocked.ToString());
    }
}
