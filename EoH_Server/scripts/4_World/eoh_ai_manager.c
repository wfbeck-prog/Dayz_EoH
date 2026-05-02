class EoH_AIManager
{
    static ref EoH_AIManager s_Instance;

    ref EoH_AITownSystemConfig m_Config;

    static EoH_AIManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_AIManager();

        return s_Instance;
    }

    void EoH_AIManager()
    {
        LoadConfig();
    }

    void LoadConfig()
    {
        string path = "$profile:EoH/WorldState/EoH_AIConfig.json";

        if (FileExist(path))
        {
            JsonFileLoader<EoH_AITownSystemConfig>.JsonLoadFile(path, m_Config);
        }
        else
        {
            m_Config = new EoH_AITownSystemConfig();
            JsonFileLoader<EoH_AITownSystemConfig>.JsonSaveFile(path, m_Config);
        }
    }

    void OnTownCaptured(string townName, int tier)
    {
        if (!m_Config || !m_Config.Enabled)
            return;

        Print("[EoH_AI] Activating AI for town: " + townName + " Tier=" + tier.ToString());
    }
};
