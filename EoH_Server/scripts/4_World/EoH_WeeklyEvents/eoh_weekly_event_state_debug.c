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

        string stateMsg = "state";
        stateMsg += " active=" + runtime.Active.ToString();
        stateMsg += " objective=" + runtime.Config.Id;
        stateMsg += " startTime=" + runtime.StartTime.ToString();
        stateMsg += " wave=" + runtime.CurrentWave.ToString();
        stateMsg += " repaired=" + runtime.AltarRelayOnline.ToString();
        stateMsg += " repairInProgress=" + runtime.AltarRepairInProgress.ToString();
        stateMsg += " rewardUnlocked=" + runtime.RewardUnlocked.ToString();
        stateMsg += " passive=" + runtime.RecoveryPassiveMode.ToString();
        stateMsg += " restored=" + runtime.RecoveryRestored.ToString();
        stateMsg += " grace=" + runtime.RecoveryGraceActive.ToString();

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
