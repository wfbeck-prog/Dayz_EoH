class EoH_DT_Core
{
    static int GetDogtagTier(int combatSkill)
    {
        if (combatSkill >= 80)
            return 3; // Hero
        if (combatSkill >= 40)
            return 2; // Bandit
        return 1; // Survivor
    }

    static string GetDogtagClassFromTier(int tier)
    {
        switch (tier)
        {
            case 3: return "Dogtag_Hero";
            case 2: return "Dogtag_Bandit";
            default: return "Dogtag_Survivor";
        }
    }
}
