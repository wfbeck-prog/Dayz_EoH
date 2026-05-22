class EoH_GroupFlagMarkerManager
{
    protected static int s_LastRefresh;
    protected static const int REFRESH_MS = 60000;
    protected static const float SCAN_RADIUS = 30000.0;

    static void Tick()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        int now = GetGame().GetTime();
        if (s_LastRefresh > 0 && now - s_LastRefresh < REFRESH_MS)
            return;

        s_LastRefresh = now;
        RefreshAllPlayers();
    }

    static void RefreshAllPlayers()
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            RefreshPlayer(player);
        }
    }

    static void RefreshPlayer(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        string playerGroupID = EoH_GroupHelper.GetGroupID(player);
        if (playerGroupID == "")
            return;

        array<Object> objects = new array<Object>();
        array<CargoBase> proxy = new array<CargoBase>();
        GetGame().GetObjectsAtPosition3D(player.GetPosition(), SCAN_RADIUS, objects, proxy);

        int sent = 0;
        foreach (Object obj : objects)
        {
            if (!obj || obj.GetType() != "TerritoryFlag")
                continue;

            string ownerGroupID = EoH_TerritoryOwnershipRegistry.GetOwnerForObject(obj);
            if (ownerGroupID == "" || ownerGroupID != playerGroupID)
                continue;

            EoH_MarkerData data = new EoH_MarkerData("EoH_GROUP_FLAG_" + obj.GetID().ToString(), "Group Territory Flag", obj.GetPosition());
            data.Category = "quest_group_flag";
            data.State = EoH_MarkerState.NORMAL;
            data.Icon = "Territory";
            data.Is3D = 1;
            data.Pulse = 0;
            data.Color = ARGB(255, 0, 180, 255);
            data.BaseColor = data.Color;
            data.Normalize();

            EoH_MarkerService.SendToPlayer(player, data);
            sent++;
        }

        if (sent > 0)
            Print("[EoH_GroupFlagMarker] Sent group flag markers player=" + player.GetIdentity().GetName() + " count=" + sent.ToString());
    }
};
