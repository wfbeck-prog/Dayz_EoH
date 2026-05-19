class EoH_QuestTravelCircleService
{
    static void SendToPlayer(PlayerBase player, EoH_QuestTravelOverlayData overlay)
    {
        if (!player || !player.GetIdentity() || !overlay)
            return;

        string circleId = overlay.GetId();
        string label = overlay.Label;
        vector center = overlay.CircleCenter;
        float radius = overlay.Radius;

        EoH_QuestTravelCircleData data = new EoH_QuestTravelCircleData(circleId, label, center, radius);
        data.Normalize();

        Param1<ref EoH_QuestTravelCircleData> param = new Param1<ref EoH_QuestTravelCircleData>(data);
        GetGame().RPCSingleParam(player, EoH_QuestTravelCircleRPC.ADD_OR_UPDATE_CIRCLE, param, true, player.GetIdentity());

        Print("[EoH_QuestCircle] Sent personal search circle player=" + player.GetIdentity().GetName() + " id=" + data.Id + " center=" + data.Center.ToString() + " radius=" + data.Radius.ToString());
    }

    static void RemoveFromPlayer(PlayerBase player, string id)
    {
        if (!player || !player.GetIdentity() || id == "")
            return;

        Param1<string> param = new Param1<string>(id);
        GetGame().RPCSingleParam(player, EoH_QuestTravelCircleRPC.REMOVE_CIRCLE, param, true, player.GetIdentity());

        Print("[EoH_QuestCircle] Removed personal search circle player=" + player.GetIdentity().GetName() + " id=" + id);
    }
};
