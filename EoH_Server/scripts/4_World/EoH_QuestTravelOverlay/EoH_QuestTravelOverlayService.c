class EoH_QuestTravelOverlayService
{
    static void ShowForPlayer(PlayerBase player, EoH_QuestTravelOverlayData overlay)
    {
        if (!player || !player.GetIdentity() || !overlay)
            return;

        EoH_QuestCircleData circle = new EoH_QuestCircleData(overlay.GetId(), overlay.Label, overlay.CircleCenter, overlay.Radius);
        circle.Color = ARGB(220, 255, 220, 80);
        circle.FillColor = ARGB(45, 255, 220, 80);
        circle.Visible = 1;
        circle.Normalize();

        EoH_QuestCircleService.SendToPlayer(player, circle);

        Print("[EoH_TravelOverlay] Show player=" + player.GetIdentity().GetName() + " id=" + overlay.GetId() + " label=" + overlay.Label + " center=" + overlay.CircleCenter.ToString() + " radius=" + overlay.Radius.ToString());
    }

    static void HideForPlayer(PlayerBase player, EoH_QuestTravelOverlayData overlay)
    {
        if (!player || !player.GetIdentity() || !overlay)
            return;

        EoH_QuestCircleService.RemoveFromPlayer(player, overlay.GetId());
        Print("[EoH_TravelOverlay] Hide player=" + player.GetIdentity().GetName() + " id=" + overlay.GetId());
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
