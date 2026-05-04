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
            Param3<vector, string, int> p;
            if (rpc.GetReadContext().Read(p))
            {
                // FIX: use town as ID
                EoH_ClientMarkers.Add(p.param2, p.param1, p.param2, p.param3, false);
            }
        }

        if (id == 777002)
        {
            // NOTE: no longer used globally, but kept for compatibility
        }

        if (id == 777003)
        {
            Param2<vector, string> p2;
            if (rpc.GetReadContext().Read(p2))
            {
                // FIX: contested uses same ID, enables pulse
                EoH_ClientMarkers.Add(p2.param2, p2.param1, p2.param2, ARGB(255,255,50,50), true);
            }
        }
    }
};
