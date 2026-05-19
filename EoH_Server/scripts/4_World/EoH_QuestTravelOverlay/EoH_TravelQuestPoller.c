class EoH_TravelQuestPoller
{
    protected static ref map<string, bool> s_PlayerMarkerVisible = new map<string, bool>();

    static void Tick()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            UpdatePlayer(player);
        }
    }

    static void UpdatePlayer(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        // Future-proof source path.
        // This now routes through the auto-source layer instead of directly depending on hardcoded config entries.
        array<ref EoH_QuestTravelOverlayData> overlays = EoH_QuestTravelOverlayAutoSource.BuildForPlayer(player);

        foreach (EoH_QuestTravelOverlayData overlay : overlays)
        {
            if (!overlay)
                continue;

            UpdatePlayerOverlay(player, overlay);
        }
    }

    static void UpdatePlayerOverlay(PlayerBase player, EoH_QuestTravelOverlayData overlay)
    {
        string stateKey = player.GetIdentity().GetPlainId() + "_" + overlay.GetId();
        bool active = HasActiveTravelQuest(player, overlay.QuestID);
        bool visible = false;

        if (s_PlayerMarkerVisible.Contains(stateKey))
            visible = s_PlayerMarkerVisible.Get(stateKey);

        if (active && !visible)
        {
            Print("[EoH_TravelQuestPoller] Active quest detected. Showing overlay player=" + player.GetIdentity().GetName() + " quest=" + overlay.QuestID.ToString() + " objective=" + overlay.ObjectiveID.ToString());
            EoH_QuestTravelOverlayService.ShowForPlayer(player, overlay);
            s_PlayerMarkerVisible.Set(stateKey, true);
            return;
        }

        if (!active && visible)
        {
            Print("[EoH_TravelQuestPoller] Quest no longer active. Hiding overlay player=" + player.GetIdentity().GetName() + " quest=" + overlay.QuestID.ToString() + " objective=" + overlay.ObjectiveID.ToString());
            EoH_QuestTravelOverlayService.HideForPlayer(player, overlay);
            s_PlayerMarkerVisible.Set(stateKey, false);
            return;
        }
    }

    static bool HasActiveTravelQuest(PlayerBase player, int questId)
    {
        if (!player)
            return false;

        ExpansionQuestModule questModule = ExpansionQuestModule.GetModuleInstance();
        if (!questModule)
        {
            Print("[EoH_TravelQuestPoller][WARN] ExpansionQuestModule unavailable.");
            return false;
        }

        ExpansionQuest quest = questModule.GetActiveQuestWithQuestID(player, questId);
        return quest != null;
    }

    static void ForceHide(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        array<ref EoH_QuestTravelOverlayData> overlays = EoH_QuestTravelOverlayAutoSource.BuildForPlayer(player);

        foreach (EoH_QuestTravelOverlayData overlay : overlays)
        {
            if (!overlay)
                continue;

            string stateKey = player.GetIdentity().GetPlainId() + "_" + overlay.GetId();
            EoH_QuestTravelOverlayService.HideForPlayer(player, overlay);
            s_PlayerMarkerVisible.Set(stateKey, false);
        }
    }
};
