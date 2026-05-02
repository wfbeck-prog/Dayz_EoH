modded class MissionServer
{
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

    void EoH_DT_UpdatePlayerDogtag(PlayerBase player)
    {
        if (!player)
            return;

        int score = EoH_DT_TerjeAdapter.GetBestSkillScore(player);
        string desired = EoH_DT_Core.ResolveDogtagClass(score);

        player.ReplaceDogtag(desired);
    }
};
