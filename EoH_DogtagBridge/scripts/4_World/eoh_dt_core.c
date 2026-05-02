class EoH_DT_Core
{
    static string ResolveDogtagClass(int score)
    {
        if (score >= 100) return "Dogtag_Hero";
        if (score >= 75) return "Dogtag_Cobalt";
        if (score >= 50) return "Dogtag_Granite";
        if (score >= 25) return "Dogtag_Silver";
        return "Dogtag_Survivor";
    }
};
