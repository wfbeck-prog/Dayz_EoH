class EoH_QuestTravelMarkerHelper
{
    static void ShowTravelQuestMarker(PlayerBase player, string questKey, string label, vector position, float radius = 500.0)
    {
        if (!player || !player.GetIdentity())
            return;

        if (questKey == "")
            questKey = label;

        string id = "EoH_QuestTravel_" + questKey;
        id.Replace(" ", "_");
        id.Replace("/", "_");
        id.Replace("\\", "_");
        id.Replace(":", "_");

        EoH_MarkerData marker = new EoH_MarkerData(id, label, position);
        marker.Category = "Quest";
        marker.State = EoH_MarkerState.ACTIVE;
        marker.Icon = "Questionmark";
        marker.Color = ARGB(255, 255, 220, 80);
        marker.BaseColor = marker.Color;
        marker.Is3D = 1;
        marker.Visible = 1;
        marker.Normalize();

        EoH_MarkerService.SendToPlayer(player, marker);

        EoH_QuestCircleData circle = new EoH_QuestCircleData(id + "_Circle", label + " Search Area", position, radius);
        circle.Color = ARGB(220, 255, 220, 80);
        circle.FillColor = ARGB(45, 255, 220, 80);
        circle.Normalize();

        EoH_QuestCircleService.SendToPlayer(player, circle);

        Print("[EoH_QuestTravelMarker] Sent personal travel quest marker id=" + id + " label=" + label + " radius=" + radius.ToString() + " player=" + player.GetIdentity().GetName());
    }

    static void RemoveTravelQuestMarker(PlayerBase player, string questKey)
    {
        if (!player || !player.GetIdentity() || questKey == "")
            return;

        string id = "EoH_QuestTravel_" + questKey;
        id.Replace(" ", "_");
        id.Replace("/", "_");
        id.Replace("\\", "_");
        id.Replace(":", "_");

        EoH_MarkerService.RemoveFromPlayer(player, id);
        EoH_QuestCircleService.RemoveFromPlayer(player, id + "_Circle");

        Print("[EoH_QuestTravelMarker] Removed personal travel quest marker id=" + id + " player=" + player.GetIdentity().GetName());
    }
};
