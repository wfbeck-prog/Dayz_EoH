class EoH_CaptureSession
{
    string TownName;
    string AttackingGroupID;
    string AttackingGroupName;
    float Progress;
    int LastTickTime;

    void EoH_CaptureSession()
    {
        Progress = 0;
        LastTickTime = 0;
    }
};

class EoH_CaptureManager
{
    static ref EoH_CaptureManager s_Instance;

    ref map<string, ref EoH_CaptureSession> m_ActiveCaptures;
    ref EoH_CaptureConfig m_Config;

    void EoH_CaptureManager()
    {
        m_ActiveCaptures = new map<string, ref EoH_CaptureSession>();
        LoadConfig();
    }

    static EoH_CaptureManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_CaptureManager();

        return s_Instance;
    }

    void LoadConfig()
    {
        string path = "$profile:EoH/WorldState/EoH_CaptureConfig.json";

        if (FileExist(path))
        {
            JsonFileLoader<EoH_CaptureConfig>.JsonLoadFile(path, m_Config);
        }
        else
        {
            m_Config = new EoH_CaptureConfig();
            JsonFileLoader<EoH_CaptureConfig>.JsonSaveFile(path, m_Config);
        }
    }

    void StartCapture(string townName, PlayerBase player)
    {
        string groupID = player.GetIdentity().GetId();
        string groupName = player.GetIdentity().GetName();

        if (!m_ActiveCaptures.Contains(townName))
        {
            EoH_CaptureSession session = new EoH_CaptureSession();
            session.TownName = townName;
            session.AttackingGroupID = groupID;
            session.AttackingGroupName = groupName;
            session.LastTickTime = GetGame().GetTime();

            m_ActiveCaptures.Insert(townName, session);

            Print("[EoH_Capture] Capture started: " + townName + " by " + groupName);
        }
    }

    void Tick()
    {
        int now = GetGame().GetTime();

        foreach (string townName, EoH_CaptureSession session : m_ActiveCaptures)
        {
            if (!session) continue;

            int delta = now - session.LastTickTime;
            session.LastTickTime = now;

            session.Progress += delta;

            float required = 1200000; // 20 min fallback

            if (session.Progress >= required)
            {
                CompleteCapture(session);
            }
        }
    }

    void CompleteCapture(EoH_CaptureSession session)
    {
        EoH_WorldStateManager.Get().SetTownOwner(session.TownName, session.AttackingGroupID, session.AttackingGroupName);

        Print("[EoH_Capture] Capture complete: " + session.TownName);

        m_ActiveCaptures.Remove(session.TownName);
    }
};
