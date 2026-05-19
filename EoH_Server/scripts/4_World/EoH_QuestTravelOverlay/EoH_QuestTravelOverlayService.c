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

        // Personal icon marker.
        EoH_MarkerService.SendToPlayer(player, marker);

        // Real client-side search-area circle overlay.
        EoH_QuestTravelCircleService.SendToPlayer(player, overlay);

        Print("[EoH_TravelOverlay] Sent personal travel search area player=" + player.GetIdentity().GetName() + " id=" + overlay.GetId() + " label=" + overlay.Label + " truePos=" + overlay.TruePosition.ToString() + " center=" + overlay.CircleCenter.ToString() + " radius=" + overlay.Radius.ToString());
    }

    static void HideForPlayer(PlayerBase player, EoH_QuestTravelOverlayData overlay)
    {
        if (!player || !player.GetIdentity() || !overlay)
            return;

        EoH_MarkerService.RemoveFromPlayer(player, overlay.GetId());
        EoH_QuestTravelCircleService.RemoveFromPlayer(player, overlay.GetId());

        Print("[EoH_TravelOverlay] Removed personal travel marker player=" + player.GetIdentity().GetName() + " id=" + overlay.GetId());
    }

    static void ShowQuestOverlay(PlayerBase player, int objectiveId)
    {
        EoH_QuestTravelOverlayData overlay = EoH_QuestTravelOverlayConfig.GetByObjective(objectiveId);

        if (!overlay)
        {
            Print("[EoH_TravelOverlay][WARN] Missing overlay config for objective=" + objectiveId.ToString());
            return;
        }

        ShowForPlayer(player, overlay);
    }

    static void HideQuestOverlay(PlayerBase player, int objectiveId)
    {
        EoH_QuestTravelOverlayData overlay = EoH_QuestTravelOverlayConfig.GetByObjective(objectiveId);

        if (!overlay)
            return;

        HideForPlayer(player, overlay);
    }

    static void ShowPrototypeForPlayer(PlayerBase player)
    {
        ShowQuestOverlay(player, 501001);
    }

    static void HidePrototypeForPlayer(PlayerBase player)
    {
        HideQuestOverlay(player, 501001);
    }
};
