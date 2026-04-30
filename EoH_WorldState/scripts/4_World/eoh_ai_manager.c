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
        MakeDirectory("$profile:EoH");
        MakeDirectory("$profile:EoH\\WorldState");

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

        if (!m_Config)
        {
            m_Config = new EoH_AITownSystemConfig();
            JsonFileLoader<EoH_AITownSystemConfig>.JsonSaveFile(path, m_Config);
        }
    }

    void GenerateAIBConfigFromCurrentWorldState()
    {
        EoH_AIBDynamicGenerator.GenerateFromWorldState(EoH_WorldStateManager.Get().GetState());
    }

    void OnTownCaptured(string townName, int tier)
    {
        if (!m_Config || !m_Config.Enabled)
            return;

        Print("[EoH_AI] Activating AI for town: " + townName + " Tier=" + tier.ToString());

        switch (tier)
        {
            case 1:
                SpawnDefensiveAI(townName);
                break;
            case 2:
                SpawnPatrolAI(townName);
                break;
            case 3:
                SpawnOutwardAI(townName);
                break;
            case 4:
                SpawnAssaultAI(townName);
                break;
        }

        GenerateAIBConfigFromCurrentWorldState();
    }

    void SpawnDefensiveAI(string town)
    {
        Print("[EoH_AI] DEFENSIVE AI: " + town);
    }

    void SpawnPatrolAI(string town)
    {
        Print("[EoH_AI] PATROL AI: " + town);
    }

    void SpawnOutwardAI(string town)
    {
        Print("[EoH_AI] OUTWARD AI: " + town);
    }

    void SpawnAssaultAI(string town)
    {
        Print("[EoH_AI] ASSAULT AI: " + town);
    }
};
