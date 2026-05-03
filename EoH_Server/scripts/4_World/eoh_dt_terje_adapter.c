class EoH_DT_TerjeAdapter
{
    static int GetBestSkillScore(PlayerBase player)
    {
        if (!player || !player.GetTerjeSkills())
            return 0;

        int best = 0;

        best = Math.Max(best, player.GetTerjeSkills().GetSkillLevel("hunt"));
        best = Math.Max(best, player.GetTerjeSkills().GetSkillLevel("surv"));
        best = Math.Max(best, player.GetTerjeSkills().GetSkillLevel("athlc"));
        best = Math.Max(best, player.GetTerjeSkills().GetSkillLevel("stlh"));
        best = Math.Max(best, player.GetTerjeSkills().GetSkillLevel("mtblsm"));
        best = Math.Max(best, player.GetTerjeSkills().GetSkillLevel("fish"));

        return best;
    }
};
