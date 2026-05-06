class EoH_TerjeRespawnCooldowns
{
    protected static ref map<string, int> s_LastSpawnByKey;

    static void Ensure()
    {
        if (!s_LastSpawnByKey)
            s_LastSpawnByKey = new map<string, int>();
    }

    static bool IsReady(PlayerBase player, string key, int cooldownSeconds)
    {
        Ensure();

        if (cooldownSeconds <= 0)
            return true;

        if (!player || !player.GetIdentity() || key == "")
            return false;

        string fullKey = player.GetIdentity().GetId() + "|" + key;
        int last = 0;
        s_LastSpawnByKey.Find(fullKey, last);

        if (last <= 0)
            return true;

        int elapsedMs = GetGame().GetTime() - last;
        return elapsedMs >= cooldownSeconds * 1000;
    }

    static int GetRemainingSeconds(PlayerBase player, string key, int cooldownSeconds)
    {
        Ensure();

        if (cooldownSeconds <= 0 || !player || !player.GetIdentity() || key == "")
            return 0;

        string fullKey = player.GetIdentity().GetId() + "|" + key;
        int last = 0;
        s_LastSpawnByKey.Find(fullKey, last);

        if (last <= 0)
            return 0;

        int elapsedMs = GetGame().GetTime() - last;
        int remaining = cooldownSeconds - (elapsedMs / 1000);
        if (remaining < 0)
            remaining = 0;

        return remaining;
    }

    static void MarkUsed(PlayerBase player, string key)
    {
        Ensure();

        if (!player || !player.GetIdentity() || key == "")
            return;

        string fullKey = player.GetIdentity().GetId() + "|" + key;
        s_LastSpawnByKey.Set(fullKey, GetGame().GetTime());
    }
}
