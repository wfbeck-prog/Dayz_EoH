class EoH_FieldReportService
{
    static void OpenForPlayer(PlayerBase player, EoH_FieldReportData report)
    {
        if (!player || !player.GetIdentity() || !report)
            return;

        Param1<ref EoH_FieldReportData> param = new Param1<ref EoH_FieldReportData>(report);
        GetGame().RPCSingleParam(player, EoH_FieldReportRPC.OPEN_REPORT, param, true, player.GetIdentity());

        Print("[EoH_FieldReport] Opened report for player=" + player.GetIdentity().GetName() + " title=" + report.Title);
    }
};
