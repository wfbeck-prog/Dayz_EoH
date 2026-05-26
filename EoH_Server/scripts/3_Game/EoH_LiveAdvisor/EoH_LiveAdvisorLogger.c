class EoH_LiveAdvisorLogger
{
    static const string PROFILE_FOLDER = "$profile:EoH/LiveAdvisor/";
    static const string SETTINGS_FILE = "$profile:EoH/LiveAdvisor/LiveAdvisorSettings.json";
    static const string EVENTS_FILE = "$profile:EoH/LiveAdvisor/events.log";
    static const string REPORT_FILE = "$profile:EoH/LiveAdvisor/latest_report.txt";

    static ref EoH_LiveAdvisorConfig m_Config;
    static ref array<string> m_RecentEvents;

    static void Init()
    {
        MakeDirectory("$profile:EoH");
        MakeDirectory(PROFILE_FOLDER);

        m_Config = new EoH_LiveAdvisorConfig();
        m_Config.Defaults();

        if (!FileExist(SETTINGS_FILE))
        {
            JsonFileLoader<EoH_LiveAdvisorConfig>.JsonSaveFile(SETTINGS_FILE, m_Config);
        }
        else
        {
            JsonFileLoader<EoH_LiveAdvisorConfig>.JsonLoadFile(SETTINGS_FILE, m_Config);
            ValidateConfig();
            JsonFileLoader<EoH_LiveAdvisorConfig>.JsonSaveFile(SETTINGS_FILE, m_Config);
        }

        if (!m_RecentEvents)
        {
            m_RecentEvents = new array<string>();
        }

        Log("SERVER_START", "EoH Live Advisor initialized", "info");
        Log("SELF_TEST", "Live Advisor file logging and report generation are online", "info");
    }

    static void ValidateConfig()
    {
        if (!m_Config)
        {
            m_Config = new EoH_LiveAdvisorConfig();
            m_Config.Defaults();
            return;
        }

        if (m_Config.ConfigVersion < 3)
        {
            m_Config.ConfigVersion = 3;
        }

        if (m_Config.ServerName == "")
        {
            m_Config.ServerName = "Echoes of Humanity Hardcore";
        }

        if (m_Config.HeartbeatSeconds < 60)
        {
            m_Config.HeartbeatSeconds = 60;
        }

        if (m_Config.MaxReportLines < 25)
        {
            m_Config.MaxReportLines = 75;
        }
    }

    static void Log(string type, string message, string severity = "info", string source = "EoH_Server")
    {
        if (m_Config && !m_Config.Enabled)
        {
            return;
        }

        string line;
        line = string.Format("[%1] [%2] [%3] %4", severity, source, type, message);

        FileHandle file = OpenFile(EVENTS_FILE, FileMode.APPEND);

        if (file)
        {
            FPrintln(file, line);
            CloseFile(file);
        }

        if (!m_RecentEvents)
        {
            m_RecentEvents = new array<string>();
        }

        m_RecentEvents.Insert(line);

        if (m_Config && m_RecentEvents.Count() > m_Config.MaxReportLines)
        {
            m_RecentEvents.RemoveOrdered(0);
        }

        WriteReport();

        if (m_Config && m_Config.LogDebugToRPT)
        {
            Print("[EoH_LiveAdvisor] " + line);
        }
    }

    static void WriteReport()
    {
        if (!m_Config || !m_Config.WriteCopyPasteReport)
        {
            return;
        }

        FileHandle report = OpenFile(REPORT_FILE, FileMode.WRITE);

        if (!report)
        {
            return;
        }

        FPrintln(report, "=== Echoes of Humanity Live Advisor Report ===");
        FPrintln(report, "Server: " + m_Config.ServerName);
        FPrintln(report, "Mode: Advisor Only");
        FPrintln(report, "ConfigVersion: " + m_Config.ConfigVersion.ToString());
        FPrintln(report, "MaxReportLines: " + m_Config.MaxReportLines.ToString());
        FPrintln(report, "EnableBunkerPanelObserver: " + m_Config.EnableBunkerPanelObserver.ToString());
        FPrintln(report, " ");

        foreach (string line : m_RecentEvents)
        {
            FPrintln(report, line);
        }

        FPrintln(report, " ");
        FPrintln(report, "Copy this full report into ChatGPT for EoH Live Advisor analysis.");

        CloseFile(report);
    }
}
