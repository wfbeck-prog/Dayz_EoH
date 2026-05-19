class EoH_TravelQuestPoller
{
    protected static ref map<string, bool> s_PlayerMarkerVisible = new map<string, bool>();
    protected static const int EOH_TRAVEL_QUEST_ID = 401001;

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

        string playerId = player.GetIdentity().GetPlainId();
        bool active = HasActiveTravelQuest(player);
        bool visible = false;

        if (s_PlayerMarkerVisible.Contains(playerId))
            visible = s_PlayerMarkerVisible.Get(playerId);

        if (active && !visible)
        {
            Print("[EoH_TravelQuestPoller] Active travel quest detected. Showing marker player=" + player.GetIdentity().GetName());
            EoH_QuestTravelOverlayService.ShowPrototypeForPlayer(player);
            s_PlayerMarkerVisible.Set(playerId, true);
            return;
        }

        if (!active && visible)
        {
            Print("[EoH_TravelQuestPoller] Travel quest no longer active. Hiding marker player=" + player.GetIdentity().GetName());
            EoH_QuestTravelOverlayService.HidePrototypeForPlayer(player);
            s_PlayerMarkerVisible.Set(playerId, false);
            return;
        }
    }

    static bool HasActiveTravelQuest(PlayerBase player)
    {
        if (!player)
            return false;

        ExpansionQuestModule questModule = ExpansionQuestModule.GetModuleInstance();
        if (!questModule)
        {
            Print("[EoH_TravelQuestPoller][WARN] ExpansionQuestModule unavailable.");
            return false;
        }

        ExpansionQuest quest = questModule.GetActiveQuestWithQuestID(player, EOH_TRAVEL_QUEST_ID);
        return quest != null;
    }

    static void ForceHide(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        string playerId = player.GetIdentity().GetPlainId();
        EoH_QuestTravelOverlayService.HidePrototypeForPlayer(player);
        s_PlayerMarkerVisible.Set(playerId, false);
    }
};
