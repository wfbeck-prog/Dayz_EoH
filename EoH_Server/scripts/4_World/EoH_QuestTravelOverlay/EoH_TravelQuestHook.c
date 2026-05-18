modded class MissionBaseWorld
{
    void EoH_LogTravelQuestHook(string eventName, ExpansionQuest quest)
    {
        int questId = -1;
        PlayerBase player;
        string playerName = "NULL";

        if (quest)
        {
            ExpansionQuestConfig cfg = quest.GetQuestConfig();
            if (cfg)
                questId = cfg.GetID();

            player = PlayerBase.Cast(quest.GetPlayer());
            if (player && player.GetIdentity())
                playerName = player.GetIdentity().GetName();
        }

        Print("[EoH_TravelQuestHook] " + eventName + " questId=" + questId.ToString() + " player=" + playerName);
    }

    bool EoH_IsTravelPrototypeQuest(ExpansionQuest quest)
    {
        if (!quest)
            return false;

        ExpansionQuestConfig cfg = quest.GetQuestConfig();
        if (!cfg)
        {
            Print("[EoH_TravelQuestHook][WARN] Quest config is null.");
            return false;
        }

        return cfg.GetID() == 401001;
    }

    PlayerBase EoH_GetQuestPlayer(ExpansionQuest quest)
    {
        if (!quest)
            return null;

        PlayerBase player = PlayerBase.Cast(quest.GetPlayer());
        if (!player)
            Print("[EoH_TravelQuestHook][WARN] Quest player is null for travel marker.");

        return player;
    }

    void EoH_ShowTravelMarkerForQuest(ExpansionQuest quest)
    {
        if (!EoH_IsTravelPrototypeQuest(quest))
            return;

        PlayerBase player = EoH_GetQuestPlayer(quest);
        if (!player || !player.GetIdentity())
            return;

        Print("[EoH_TravelQuestHook] Show marker for Q_401001/O_501001 player=" + player.GetIdentity().GetName());
        EoH_QuestTravelOverlayService.ShowPrototypeForPlayer(player);
    }

    void EoH_HideTravelMarkerForQuest(ExpansionQuest quest)
    {
        if (!EoH_IsTravelPrototypeQuest(quest))
            return;

        PlayerBase player = EoH_GetQuestPlayer(quest);
        if (!player || !player.GetIdentity())
            return;

        Print("[EoH_TravelQuestHook] Hide marker for Q_401001/O_501001 player=" + player.GetIdentity().GetName());
        EoH_QuestTravelOverlayService.HidePrototypeForPlayer(player);
    }

    override void Expansion_OnQuestStart(ExpansionQuest quest)
    {
        super.Expansion_OnQuestStart(quest);
        EoH_LogTravelQuestHook("OnQuestStart", quest);
        EoH_ShowTravelMarkerForQuest(quest);
    }

    override void Expansion_OnQuestContinue(ExpansionQuest quest)
    {
        super.Expansion_OnQuestContinue(quest);
        EoH_LogTravelQuestHook("OnQuestContinue", quest);
        EoH_ShowTravelMarkerForQuest(quest);
    }

    override void Expansion_OnQuestCancel(ExpansionQuest quest)
    {
        super.Expansion_OnQuestCancel(quest);
        EoH_LogTravelQuestHook("OnQuestCancel", quest);
        EoH_HideTravelMarkerForQuest(quest);
    }

    override void Expansion_OnQuestCompletion(ExpansionQuest quest)
    {
        super.Expansion_OnQuestCompletion(quest);
        EoH_LogTravelQuestHook("OnQuestCompletion", quest);
        EoH_HideTravelMarkerForQuest(quest);
    }

    override void Expansion_OnQuestObjectivesComplete(ExpansionQuest quest)
    {
        super.Expansion_OnQuestObjectivesComplete(quest);
        EoH_LogTravelQuestHook("OnQuestObjectivesComplete", quest);
        EoH_HideTravelMarkerForQuest(quest);
    }
};
