class EoH_LiveAdvisorLogger
{
    static const string PROFILE_FOLDER = "$profile:EoH/LiveAdvisor/";
    static const string SETTINGS_FILE = "$profile:EoH/LiveAdvisor/LiveAdvisorSettings.json";
    static const string EVENTS_FILE = "$profile:EoH/LiveAdvisor/events.log";

    static ref EoH_LiveAdvisorConfig m_Config;

    static void Init()
    {
        MakeDirectory("$profile:EoH");
        MakeDirectory(PROFILE_FOLDER);

        if (!FileExist(SETTINGS_FILE))
        {
            m_Config = new EoH_LiveAdvisorConfig();
            m_Config.Defaults();
            JsonFileLoader<EoH_LiveAdvisorConfig>.JsonSaveFile(SETTINGS_FILE, m_Config);
        }
        else
        {
            m_Config = new EoH_LiveAdvisorConfig();
            JsonFileLoader<EoH_LiveAdvisorConfig>.JsonLoadFile(SETTINGS_FILE, m_Config);
        }

        Log("SERVER_START", "EoH Live Advisor initialized", "info");
    }

    static void Log(string type, string message, string severity = "info", string source = "EoH_Server")
    {
        string line;
        line = string.Format("[%1] [%2] [%3] %4", severity, source, type, message);

        FileHandle file = OpenFile(EVENTS_FILE, FileMode.APPEND);

        if (file)
        {
            FPrintln(file, line);
            CloseFile(file);
        }

        if (m_Config && m_Config.LogDebugToRPT)
        {
            Print("[EoH_LiveAdvisor] " + line);
        }
    }
}
