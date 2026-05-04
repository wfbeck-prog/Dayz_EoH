modded class MissionGameplay
{
    override void OnEvent(EventType eventTypeId, Param params)
    {
        super.OnEvent(eventTypeId, params);

        if (eventTypeId != EventTypeID.RPC)
            return;

        ScriptRPC rpc = ScriptRPC.Cast(params);
        if (!rpc) return;

        int id = rpc.GetID();

        if (id == 777001)
        {
            Param2<vector, string> p;
            if (rpc.GetReadContext().Read(p))
            {
                EoH_ClientMarkers.Add(p.param1, p.param2, ARGB(255,255,255,255));
            }
        }

        if (id == 777002)
        {
            EoH_ClientMarkers.Clear();
        }
    }
};
