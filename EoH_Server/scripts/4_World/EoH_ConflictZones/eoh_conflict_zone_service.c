class EoH_ConflictZoneEntry
{
    string Id;
    vector Center;
    int ExpiresAt;

    void EoH_ConflictZoneEntry(string id = "", vector center = "0 0 0", int expiresAt = 0)
    {
        Id = id;
        Center = center;
        ExpiresAt = expiresAt;
    }
};

class EoH_ConflictZoneService
{
    static const float DEFAULT_RADIUS = 600.0;
    static const int DEFAULT_LIFETIME_MS = 1200000;
    static const int COLOR = ARGB(185, 255, 45, 45);
    static const string LABEL = "Recent Conflict";

    protected static ref array<ref EoH_ConflictZoneEntry> s_Zones = new array<ref EoH_ConflictZoneEntry>();

    static void CreateSurvivorConflictZone(PlayerBase victim, PlayerBase sourcePlayer)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!victim || !sourcePlayer)
            return;

        if (!victim.GetIdentity() || !sourcePlayer.GetIdentity())
            return;

        if (victim == sourcePlayer)
            return;

        vector center = victim.GetPosition();
        EoH_ConflictZoneEntry existing = FindNearbyZone(center, DEFAULT_RADIUS);

        if (existing)
        {
            existing.Center = center;
            existing.ExpiresAt = GetGame().GetTime() + DEFAULT_LIFETIME_MS;
            BroadcastZone(existing.Id, LABEL, center, DEFAULT_RADIUS, DEFAULT_LIFETIME_MS);
            Print("[EoH_ConflictZone] Refreshed zone id=" + existing.Id + " center=" + center.ToString());
            return;
        }

        string id = BuildZoneId(center);
        EoH_ConflictZoneEntry zone = new EoH_ConflictZoneEntry(id, center, GetGame().GetTime() + DEFAULT_LIFETIME_MS);
        s_Zones.Insert(zone);

        BroadcastZone(id, LABEL, center, DEFAULT_RADIUS, DEFAULT_LIFETIME_MS);
    }

    static EoH_ConflictZoneEntry FindNearbyZone(vector center, float radius)
    {
        CleanupExpiredZoneEntries();

        foreach (EoH_ConflictZoneEntry zone : s_Zones)
        {
            if (!zone)
                continue;

            if (vector.Distance(center, zone.Center) <= radius)
                return zone;
        }

        return null;
    }

    static void CleanupExpiredZoneEntries()
    {
        int now = GetGame().GetTime();

        for (int i = s_Zones.Count() - 1; i >= 0; i--)
        {
            EoH_ConflictZoneEntry zone = s_Zones.Get(i);
            if (!zone || zone.ExpiresAt <= now)
                s_Zones.Remove(i);
        }
    }

    static string BuildZoneId(vector center)
    {
        return "EoH_CONFLICT_" + Math.Round(center[0]).ToString() + "_" + Math.Round(center[2]).ToString() + "_" + GetGame().GetTime().ToString();
    }

    static void BroadcastZone(string id, string label, vector center, float radius, int lifetimeMs)
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        EoH_QuestTravelCircleData data = new EoH_QuestTravelCircleData(id, label, center, radius);
        data.Color = COLOR;
        data.Visible = 1;
        data.Normalize();

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            Param1<ref EoH_QuestTravelCircleData> param = new Param1<ref EoH_QuestTravelCircleData>(data);
            GetGame().RPCSingleParam(player, EoH_QuestTravelCircleRPC.ADD_OR_UPDATE_CIRCLE, param, true, player.GetIdentity());
        }

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RemoveZoneFromAllPlayersIfExpired, lifetimeMs, false, id);

        Print("[EoH_ConflictZone] Broadcast zone id=" + id + " center=" + center.ToString() + " radius=" + radius.ToString());
    }

    static void RemoveZoneFromAllPlayersIfExpired(string id)
    {
        if (!GetGame() || !GetGame().IsServer() || id == "")
            return;

        int now = GetGame().GetTime();

        foreach (EoH_ConflictZoneEntry zone : s_Zones)
        {
            if (!zone || zone.Id != id)
                continue;

            if (zone.ExpiresAt > now)
            {
                int delay = zone.ExpiresAt - now;
                GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RemoveZoneFromAllPlayersIfExpired, delay, false, id);
                return;
            }
        }

        RemoveZoneFromAllPlayers(id);
    }

    static void RemoveZoneFromAllPlayers(string id)
    {
        if (!GetGame() || !GetGame().IsServer() || id == "")
            return;

        for (int i = s_Zones.Count() - 1; i >= 0; i--)
        {
            EoH_ConflictZoneEntry zone = s_Zones.Get(i);
            if (zone && zone.Id == id)
                s_Zones.Remove(i);
        }

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            Param1<string> param = new Param1<string>(id);
            GetGame().RPCSingleParam(player, EoH_QuestTravelCircleRPC.REMOVE_CIRCLE, param, true, player.GetIdentity());
        }

        Print("[EoH_ConflictZone] Removed zone id=" + id);
    }
};
