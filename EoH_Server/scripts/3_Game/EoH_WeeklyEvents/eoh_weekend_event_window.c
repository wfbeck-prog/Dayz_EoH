class EoH_WeekendEventWindow
{
    static const bool ENABLE_WEEKEND_EVENT_WINDOW_GATE = false;

    static bool IsGateEnabled()
    {
        return ENABLE_WEEKEND_EVENT_WINDOW_GATE;
    }

    static bool IsWeekendEventWindowOpen()
    {
        return true;
    }

    static string GetClosedMessage()
    {
        return "Weekend event intel is encrypted. Signals open Friday 6 PM Eastern and close Sunday 11 PM Eastern.";
    }

    static bool CanUseWeekendIntel()
    {
        return true;
    }

    static bool CanStartWeekendEvent()
    {
        return true;
    }
}
