class EoH_DT_Core
{
    static string ResolveDogtagClass(int exp)
    {
        if (exp >= 2000)
            return "Dogtag_Hero";

        if (exp >= 1500)
            return "Dogtag_Cobalt";

        if (exp >= 1000)
            return "Dogtag_Granite";

        if (exp >= 500)
            return "Dogtag_Silver";

        return "Dogtag_Survivor";
    }
};
