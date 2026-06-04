class EoH_ConflictZoneService
{
    static const float DEFAULT_RADIUS = 500.0;
    static const int DEFAULT_LIFETIME_MS = 900000; // 15 minutes
    static const int COLOR = ARGB(180, 255, 45, 45);

    static void CreateSurvivorKillZone(PlayerBase victim, PlayerBase killer)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!victim || !killer)
            return;

        if (!victim.GetIdentity() || !killer.GetIdentity())
            return;

        if (victim == killer)
            return;

        vector center = victim.GetPosition();
        string id = BuildZoneId(victim, center);
        string label = "Conflict Zone";

        BroadcastZone(id, label, center, DEFAULT_RADIUS, DEFAULT_LIFETIME_MS);
    }

    static string BuildZoneId(PlayerBase victim, vector center)
    {
        string name = "unknown";
        if (victim && victim.GetIdentity())
            name = victim.GetIdentity().GetId();

        return "EoH_CONFLICT_" + name + "_" + Math.Round(center[0]).ToString() + "_" + Math.Round(center[2]).ToString() + "_" + GetGame().GetTime().ToString();
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

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RemoveZoneFromAllPlayers, lifetimeMs, false, id);

        Print("[EoH_ConflictZone] Created conflict zone id=" + id + " center=" + center.ToString() + " radius=" + radius.ToString() + " lifetimeMs=" + lifetimeMs.ToString());
    }

    static void RemoveZoneFromAllPlayers(string id)
    {
        if (!GetGame() || !GetGame().IsServer() || id == "")
            return;

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

        Print("[EoH_ConflictZone] Removed conflict zone id=" + id);
    }
};
