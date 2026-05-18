class EoH_QuestTravelOverlayService
{
    static void ShowForPlayer(PlayerBase player, EoH_QuestTravelOverlayData overlay)
    {
        if (!player || !player.GetIdentity() || !overlay)
            return;

        EoH_MarkerData marker = new EoH_MarkerData(overlay.GetId(), overlay.Label, overlay.CircleCenter);
        marker.Category = "quest_travel";
        marker.State = "active";
        marker.Owner = player.GetIdentity().GetPlainId();
        marker.Color = ARGB(220, 255, 220, 80);
        marker.BaseColor = marker.Color;
        marker.Pulse = 1;
        marker.Is3D = 0;
        marker.Icon = "Map Marker";
        marker.Visible = 1;
        marker.Normalize();

        EoH_MarkerService.SendToPlayer(player, marker);

        Print("[EoH_TravelOverlay] Sent personal travel marker player=" + player.GetIdentity().GetName() + " id=" + overlay.GetId() + " label=" + overlay.Label + " center=" + overlay.CircleCenter.ToString() + " radius=" + overlay.Radius.ToString());
    }

    static void HideForPlayer(PlayerBase player, EoH_QuestTravelOverlayData overlay)
    {
        if (!player || !player.GetIdentity() || !overlay)
            return;

        EoH_MarkerService.RemoveFromPlayer(player, overlay.GetId());
        Print("[EoH_TravelOverlay] Removed personal travel marker player=" + player.GetIdentity().GetName() + " id=" + overlay.GetId());
    }

    static void ShowPrototypeForPlayer(PlayerBase player)
    {
        EoH_QuestTravelOverlayData overlay = EoH_QuestTravelOverlayConfig.GetByObjective(501001);
        ShowForPlayer(player, overlay);
    }

    static void HidePrototypeForPlayer(PlayerBase player)
    {
        EoH_QuestTravelOverlayData overlay = EoH_QuestTravelOverlayConfig.GetByObjective(501001);
        HideForPlayer(player, overlay);
    }
};
