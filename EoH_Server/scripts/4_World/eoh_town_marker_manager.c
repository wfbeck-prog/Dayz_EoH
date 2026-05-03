class EoH_TownMarkerManager
{
    static int GetGroupColor(string groupName)
    {
        if (groupName == "")
            return ARGB(255, 200, 200, 200);

        int len = groupName.Length();

        switch (len % 8)
        {
            case 0: return ARGB(255, 255, 80, 80);
            case 1: return ARGB(255, 80, 160, 255);
            case 2: return ARGB(255, 80, 255, 120);
            case 3: return ARGB(255, 255, 200, 80);
            case 4: return ARGB(255, 180, 80, 255);
            case 5: return ARGB(255, 80, 255, 220);
            case 6: return ARGB(255, 255, 120, 200);
            default: return ARGB(255, 200, 200, 200);
        }

        return ARGB(255, 200, 200, 200);
    }

    static void SendMarkerToPlayer(PlayerBase player, EoH_TownMarkerData data)
    {
        if (!GetGame() || !GetGame().IsServer() || !player || !player.GetIdentity())
            return;

        data.Normalize();

        Param1<ref EoH_TownMarkerData> param = new Param1<ref EoH_TownMarkerData>(data);
        GetGame().RPCSingleParam(player, EoH_TownMarkerRPC.ADD_OR_UPDATE_TOWN_MARKER, param, true, player.GetIdentity());
    }

    static void RemoveMarkerFromPlayer(PlayerBase player, string markerId)
    {
        if (!GetGame() || !GetGame().IsServer() || !player || !player.GetIdentity())
            return;

        Param1<string> param = new Param1<string>(markerId);
        GetGame().RPCSingleParam(player, EoH_TownMarkerRPC.REMOVE_TOWN_MARKER, param, true, player.GetIdentity());
    }

    static string GetMarkerId(string townName)
    {
        string id = "EoH_TOWN_" + townName;
        id.Replace(" ", "_");
        return id;
    }

    static vector GetTownPosition(string townName)
    {
        EoH_CaptureTownConfig cfg = EoH_CaptureManager.Get().GetTownConfig(townName);
        if (cfg)
            return cfg.GetRelayVector();

        return "0 0 0";
    }
};
