class EoH_WeeklyEventStateDebug
{
    protected static int s_LastLogTime;

    static void LogNoRuntime(int now)
    {
        if (ShouldSkip(now))
            return;

        string stateMsg = "state active=false objective=none";
        Print("[EoH_WeeklyEventState] " + stateMsg);
        EoH_LiveAdvisorActivity.LogActivity("weekly_event", stateMsg);
    }

    static void LogRuntime(EoH_EventObjectiveRuntime runtime, int now)
    {
        if (ShouldSkip(now))
            return;

        if (!runtime || !runtime.Config)
        {
            LogNoRuntime(now);
            return;
        }

        string stateMsg = "state active=" + runtime.Active.ToString()
            + " objective=" + runtime.Config.Id
            + " startTime=" + runtime.StartTime.ToString()
            + " wave=" + runtime.CurrentWave.ToString()
            + " repaired=" + runtime.AltarRelayOnline.ToString()
            + " repairInProgress=" + runtime.AltarRepairInProgress.ToString()
            + " rewardUnlocked=" + runtime.RewardUnlocked.ToString()
            + " passive=" + runtime.RecoveryPassiveMode.ToString()
            + " restored=" + runtime.RecoveryRestored.ToString()
            + " grace=" + runtime.RecoveryGraceActive.ToString();

        Print("[EoH_WeeklyEventState] " + stateMsg);
        EoH_LiveAdvisorActivity.LogActivity("weekly_event", stateMsg);
    }

    protected static bool ShouldSkip(int now)
    {
        if (s_LastLogTime > 0 && now - s_LastLogTime < 60000)
            return true;

        s_LastLogTime = now;
        return false;
    }
}
