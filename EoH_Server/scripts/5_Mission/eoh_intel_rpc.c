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

        Param2<vector, string> p;
        if (!rpc.GetReadContext().Read(p))
            return;

        vector pos = p.param1;
        string markerId = p.param2;

        // Remove marker
        if (id == 777099)
        {
            EoH_ClientMarkers.Remove(markerId);
            return;
        }

        int color = ARGB(255,255,255,255);
        bool pulse = false;

        switch (id)
        {
            case 777001: // base
                color = ARGB(120,150,150,150);
                break;

            case 777002: // intel reveal
                color = ARGB(255,255,255,255);
                break;

            case 777003: // contested
                color = ARGB(255,255,50,50);
                pulse = true;
                break;

            case 777004: // capturing
                color = ARGB(255,255,220,80);
                break;

            case 777005: // owned
                color = ARGB(255,80,255,120);
                break;

            case 777010: // trader
                color = ARGB(255,80,150,255);
                break;
        }

        EoH_ClientMarkers.Add(markerId, pos, markerId, color, pulse);
    }
}
