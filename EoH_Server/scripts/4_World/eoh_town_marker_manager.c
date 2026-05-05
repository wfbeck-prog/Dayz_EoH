class EoH_TownMarkerManager
{
    static int GetGroupColor(string groupName)
    {
        if (groupName == "" || groupName == "Unclaimed")
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

    static void UpdateTownMarker(string townName, string owner)
    {
        EoH_MarkerData data = BuildTownMarker(townName, owner, EoH_MarkerState.OWNED, 0, GetGroupColor(owner));
        EoH_MarkerService.Broadcast(data);
    }

    static void UpdateCapturingMarker(string townName, string owner)
    {
        EoH_MarkerData data = BuildTownMarker(townName, owner, EoH_MarkerState.CAPTURING, 1, GetGroupColor(owner));
        EoH_MarkerService.Broadcast(data);
    }

    static void UpdateContestedMarker(string townName, string owner)
    {
        EoH_MarkerData data = BuildTownMarker(townName, owner, EoH_MarkerState.CONTESTED, 1, ARGB(255, 255, 50, 50));
        EoH_MarkerService.Broadcast(data);
    }

    static void ClearContestedMarker(string townName)
    {
        RemoveMarkerFromAll(GetMarkerId(townName));
    }

    static void RemoveMarkerFromAll(string markerId)
    {
        EoH_MarkerService.RemoveFromAll(markerId);
    }

    static void RemoveMarkerFromPlayer(PlayerBase player, string markerId)
    {
        EoH_MarkerService.RemoveFromPlayer(player, markerId);
    }

    static void SendMarkerToPlayer(PlayerBase player, EoH_MarkerData data)
    {
        EoH_MarkerService.SendToPlayer(player, data);
    }

    static EoH_MarkerData BuildTownMarker(string townName, string owner, string state, int pulse, int color)
    {
        EoH_MarkerData data = new EoH_MarkerData(GetMarkerId(townName), townName, GetTownPosition(townName));
        data.Category = EoH_MarkerCategory.TOWN;
        data.State = state;
        data.Owner = owner;
        data.Color = color;
        data.BaseColor = color;
        data.Pulse = pulse;
        data.Is3D = 0;
        data.Icon = "Flag";
        data.Visible = 1;
        data.Normalize();
        return data;
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
