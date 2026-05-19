class EoH_FieldReportService
{
    static void OpenForPlayer(PlayerBase player, EoH_FieldReportData report)
    {
        if (!player || !player.GetIdentity() || !report)
            return;

        Param1<ref EoH_FieldReportData> param = new Param1<ref EoH_FieldReportData>(report);
        GetGame().RPCSingleParam(player, EoH_FieldReportRPC.OPEN_REPORT, param, true, player.GetIdentity());

        // Fallback until the custom UI open path is confirmed on the client.
        // This preserves the readable report content and proves delivery/data generation.
        string fallback = report.Subtitle + "\n" + report.Body;
        EoH_Notifications.SendToPlayer(player, report.Title, fallback, "Info", 0, 18.0);

        Print("[EoH_FieldReport] Sent report for player=" + player.GetIdentity().GetName() + " title=" + report.Title);
    }
};
