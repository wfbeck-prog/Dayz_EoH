class EoH_LiveAdvisorActivity
{
    protected static ref map<string, float> m_LastActivityLogTimes;

    static void LogActivity(string systemName, string message)
    {
        if (!EoH_LiveAdvisorLogger.m_Config)
            return;

        if (!EoH_LiveAdvisorLogger.m_Config.EnableActivityCorrelation)
            return;

        if (!m_LastActivityLogTimes)
        {
            m_LastActivityLogTimes = new map<string, float>;
        }

        float now = GetGame().GetTime() / 1000.0;
        float cooldown = EoH_LiveAdvisorLogger.m_Config.ActivityReportCooldownSeconds;

        if (m_LastActivityLogTimes.Contains(systemName))
        {
            float lastTime = m_LastActivityLogTimes.Get(systemName);

            if (now - lastTime < cooldown)
            {
                return;
            }
        }

        m_LastActivityLogTimes.Set(systemName, now);

        EoH_LiveAdvisorLogger.Log(
            "SYSTEM_ACTIVITY",
            "System=" + systemName + " " + message,
            "info",
            "EoH_Activity"
        );
    }
}
