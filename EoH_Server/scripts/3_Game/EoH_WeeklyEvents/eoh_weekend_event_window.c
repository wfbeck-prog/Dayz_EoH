class EoH_WeekendEventWindow
{
    static const bool ENABLE_WEEKEND_EVENT_WINDOW_GATE = false;

    static const int FRIDAY = 5;
    static const int SATURDAY = 6;
    static const int SUNDAY = 0;

    static const int FRIDAY_START_HOUR_ET = 18;
    static const int SUNDAY_END_HOUR_ET = 23;

    static bool IsGateEnabled()
    {
        return ENABLE_WEEKEND_EVENT_WINDOW_GATE;
    }

    static bool IsWeekendEventWindowOpen()
    {
        if (!ENABLE_WEEKEND_EVENT_WINDOW_GATE)
            return true;

        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;

        GetYearMonthDayUTC(year, month, day);
        GetHourMinuteSecondUTC(hour, minute, second);

        int easternHour = hour - GetEasternUtcOffsetHours(year, month, day, hour);
        int easternDay = GetDayOfWeek(year, month, day);

        if (easternHour < 0)
        {
            easternHour += 24;
            easternDay--;
            if (easternDay < 0)
                easternDay = 6;
        }
        else if (easternHour >= 24)
        {
            easternHour -= 24;
            easternDay++;
            if (easternDay > 6)
                easternDay = 0;
        }

        if (easternDay == FRIDAY)
            return easternHour >= FRIDAY_START_HOUR_ET;

        if (easternDay == SATURDAY)
            return true;

        if (easternDay == SUNDAY)
            return easternHour <= SUNDAY_END_HOUR_ET;

        return false;
    }

    static string GetClosedMessage()
    {
        return "Weekend event intel is encrypted. Signals open Friday 6 PM Eastern and close Sunday 11 PM Eastern.";
    }

    static bool CanUseWeekendIntel(PlayerBase player)
    {
        if (IsWeekendEventWindowOpen())
            return true;

        if (player)
            EoH_Notifications.SendToPlayer(player, "WEEKEND EVENTS", GetClosedMessage());

        return false;
    }

    static bool CanStartWeekendEvent()
    {
        return IsWeekendEventWindowOpen();
    }

    static int GetEasternUtcOffsetHours(int year, int month, int day, int utcHour)
    {
        if (IsEasternDaylightTime(year, month, day, utcHour))
            return 4;

        return 5;
    }

    static bool IsEasternDaylightTime(int year, int month, int day, int utcHour)
    {
        if (month < 3 || month > 11)
            return false;

        if (month > 3 && month < 11)
            return true;

        int dow = GetDayOfWeek(year, month, day);

        if (month == 3)
        {
            int secondSunday = GetNthSundayOfMonth(year, 3, 2);
            if (day > secondSunday)
                return true;
            if (day < secondSunday)
                return false;

            return utcHour >= 7;
        }

        if (month == 11)
        {
            int firstSunday = GetNthSundayOfMonth(year, 11, 1);
            if (day < firstSunday)
                return true;
            if (day > firstSunday)
                return false;

            return utcHour < 6;
        }

        return false;
    }

    static int GetNthSundayOfMonth(int year, int month, int nth)
    {
        int count = 0;
        for (int d = 1; d <= 31; d++)
        {
            if (!IsValidDate(year, month, d))
                break;

            if (GetDayOfWeek(year, month, d) == SUNDAY)
            {
                count++;
                if (count == nth)
                    return d;
            }
        }

        return 1;
    }

    static bool IsValidDate(int year, int month, int day)
    {
        if (day < 1)
            return false;

        if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
            return day <= 31;

        if (month == 4 || month == 6 || month == 9 || month == 11)
            return day <= 30;

        if (month == 2)
        {
            if (IsLeapYear(year))
                return day <= 29;
            return day <= 28;
        }

        return false;
    }

    static bool IsLeapYear(int year)
    {
        if (year % 400 == 0)
            return true;
        if (year % 100 == 0)
            return false;
        return year % 4 == 0;
    }

    static int GetDayOfWeek(int year, int month, int day)
    {
        int y = year;
        int m = month;

        if (m < 3)
        {
            m += 12;
            y--;
        }

        int k = y % 100;
        int j = y / 100;
        int h = (day + Math.Floor((13 * (m + 1)) / 5) + k + Math.Floor(k / 4) + Math.Floor(j / 4) + (5 * j)) % 7;

        int dow = h - 1;
        if (dow < 0)
            dow = 6;

        return dow;
    }
}
