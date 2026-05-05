modded class PlayerBase
{
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        if (rpc_type == EoH_MarkerRPC.ADD_OR_UPDATE_MARKER)
        {
            Param1<ref EoH_MarkerData> data;
            if (!ctx.Read(data))
                return;

            if (!data || !data.param1)
                return;

            EoH_MarkerManager.Get().Upsert(data.param1);
        }
        else if (rpc_type == EoH_MarkerRPC.REMOVE_MARKER)
        {
            Param1<string> id;
            if (!ctx.Read(id))
                return;

            if (!id || id.param1 == "")
                return;

            EoH_MarkerManager.Get().Remove(id.param1);
        }
    }
};
