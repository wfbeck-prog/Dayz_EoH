class EoH_LiveAdvisorPlayerLoad
{
    protected static float m_LastClientReadyTime;
    protected static string m_LastClientReadyName;

    static void MarkClientReady(string playerName)
    {
        m_LastClientReadyTime = GetGame().GetTime() / 1000.0;
        m_LastClientReadyName = playerName;

        EoH_LiveAdvisorActivity.LogActivity("player_session", "loadin_window_started name=" + playerName);
    }

    static bool IsInLoadInGraceWindow()
    {
        if (!EoH_LiveAdvisorLogger.m_Config)
            return false;

        if (!EoH_LiveAdvisorLogger.m_Config.EnablePlayerLoadInGrace)
            return false;

        if (m_LastClientReadyTime <= 0)
            return false;

        float now = GetGame().GetTime() / 1000.0;
        float graceSeconds = EoH_LiveAdvisorLogger.m_Config.PlayerLoadInGraceSeconds;

        return now - m_LastClientReadyTime <= graceSeconds;
    }

    static string GetLastClientReadyName()
    {
        if (m_LastClientReadyName == "")
            return "unknown";

        return m_LastClientReadyName;
    }

    static float GetSecondsSinceClientReady()
    {
        if (m_LastClientReadyTime <= 0)
            return -1;

        float now = GetGame().GetTime() / 1000.0;
        return now - m_LastClientReadyTime;
    }
}
