class EoH_SCB_Skill
{
    static int GetPlayerSkillLevel(PlayerBase player)
    {
        if (!player)
            return 0;

        // Basic fallback (replace with actual Terje skill call if needed)
        if (player.GetTerjeSkills())
            return player.GetTerjeSkills().GetLevel();

        return 0;
    }

    static string GetContainerClassForPlayer(PlayerBase player, EoH_SCB_Config config)
    {
        int level = GetPlayerSkillLevel(player);

        if (level >= 75 && config.Tier4x4 && config.Level4x4)
            return "A6_SecureContainer_4x4";

        if (level >= 25 && config.Tier3x3 && config.Level3x3)
            return "A6_SecureContainer_3x3";

        if (config.Tier2x2 && config.Level2x2)
            return "A6_SecureContainer_2x2";

        return "";
    }
};
