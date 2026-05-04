modded class PlayerBase
{
    override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, target, rpc_type, ctx);

        if (rpc_type == EoH_TownMarkerRPC.ADD_OR_UPDATE_TOWN_MARKER)
        {
            Param1<ref EoH_TownMarkerData> p;
            if (ctx.Read(p))
            {
                EoH_ClientMarkerStore.AddOrUpdate(p.param1);
            }
            return;
        }

        if (rpc_type == EoH_TownMarkerRPC.REMOVE_TOWN_MARKER)
        {
            Param1<string> p2;
            if (ctx.Read(p2))
            {
                EoH_ClientMarkerStore.Remove(p2.param1);
            }
            return;
        }
    }
};
