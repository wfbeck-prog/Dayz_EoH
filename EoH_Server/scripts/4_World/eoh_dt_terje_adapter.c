class EoH_DT_TerjeAdapter
{
    static int GetBestSkillScore(PlayerBase player)
    {
        if (!player || !player.GetTerjeSkills())
            return 0;

        // Use Survival EXP only (aligned with SCB system)
        return player.GetTerjeSkills().GetSkillExperience("surv");
    }
};
