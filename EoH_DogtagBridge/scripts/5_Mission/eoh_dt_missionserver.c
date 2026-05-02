modded class MissionServer
{
    protected bool m_EoH_DT_LiveUpdatesStarted;

    override void OnInit()
    {
        super.OnInit();

        EoH_DT_StartLiveUpdates();
    }

    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);

        EoH_DT_UpdatePlayerDogtag(player);
    }

    override void EquipCharacter(MenuDefaultCharacterData char_data)
    {
        super.EquipCharacter(char_data);

        if (m_player)
            EoH_DT_UpdatePlayerDogtag(m_player);
    }

    protected void EoH_DT_StartLiveUpdates()
    {
        if (m_EoH_DT_LiveUpdatesStarted)
            return;

        if (!GetGame() || !GetGame().IsServer())
            return;

        // Re-check all connected players every 60 seconds so dogtags evolve as Terje skills increase.
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_DT_UpdateAllPlayerDogtags, 60000, true);
        m_EoH_DT_LiveUpdatesStarted = true;

        Print("[EoH_DogtagBridge] Live dogtag skill updates started.");
    }

    void EoH_DT_UpdateAllPlayerDogtags()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity() || !player.IsAlive())
                continue;

            EoH_DT_UpdatePlayerDogtag(player);
        }
    }

    void EoH_DT_UpdatePlayerDogtag(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        int score = EoH_DT_TerjeAdapter.GetBestSkillScore(player);
        string desired = EoH_DT_Core.ResolveDogtagClass(score);

        if (desired == "")
            return;

        player.ReplaceDogtag(desired);
    }
};
