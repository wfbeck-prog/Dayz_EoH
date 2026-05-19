class EoH_QuestTravelOverlayService
{
    static void ShowForPlayer(PlayerBase player, EoH_QuestTravelOverlayData overlay)
    {
        if (!player || !player.GetIdentity() || !overlay)
            return;

        // Circle-only travel overlay.
        // Do not send an Expansion icon marker because the icon can reveal the exact task point.
        EoH_QuestTravelCircleService.SendToPlayer(player, overlay);

        Print("[EoH_TravelOverlay] Sent personal travel circle player=" + player.GetIdentity().GetName() + " id=" + overlay.GetId() + " label=" + overlay.Label + " truePos=" + overlay.TruePosition.ToString() + " center=" + overlay.CircleCenter.ToString() + " radius=" + overlay.Radius.ToString());
    }

    static void HideForPlayer(PlayerBase player, EoH_QuestTravelOverlayData overlay)
    {
        if (!player || !player.GetIdentity() || !overlay)
            return;

        // Remove both paths so old icon markers from earlier builds are cleaned up too.
        EoH_MarkerService.RemoveFromPlayer(player, overlay.GetId());
        EoH_QuestTravelCircleService.RemoveFromPlayer(player, overlay.GetId());

        Print("[EoH_TravelOverlay] Removed personal travel overlay player=" + player.GetIdentity().GetName() + " id=" + overlay.GetId());
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
