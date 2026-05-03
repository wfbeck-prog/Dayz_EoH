class EoH_SCB_Skill
{
    static int GetPlayerSkillExp(PlayerBase player)
    {
        if (!player || !player.GetTerjeSkills())
            return 0;

        return player.GetTerjeSkills().GetSkillExperience("surv");
    }

    static string GetContainerClassForPlayer(PlayerBase player, EoH_SCB_Config config)
    {
        int exp = GetPlayerSkillExp(player);

        if (exp >= 2000 && config.Tier4x4)
            return "A6_SecureContainer_4x4";

        if (exp >= 500 && config.Tier3x3)
            return "A6_SecureContainer_3x3";

        if (config.Tier2x2)
            return "A6_SecureContainer_2x2";

        return "";
    }
};
