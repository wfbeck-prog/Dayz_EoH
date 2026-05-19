modded class PlayerBase
{
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        if (rpc_type == EoH_FieldReportRPC.OPEN_REPORT)
        {
            Param1<ref EoH_FieldReportData> data;
            if (!ctx.Read(data))
                return;

            if (!data || !data.param1)
                return;

            EoH_FieldReportMenu.Open(data.param1);
        }
    }
};
