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

    static string GetIconForState(string state, string owner)
    {
        if (state == EoH_MarkerState.CAPTURING)
            return "Radio";

        if (state == EoH_MarkerState.CONTESTED)
            return "Exclamationmark";

        if (state == EoH_MarkerState.OWNED && owner != "" && owner != "Unclaimed")
            return "Territory";

        return "Territory";
    }

    static void UpdateTownMarker(string townName, string owner)
    {
        EoH_MarkerData data = BuildTownMarker(townName, owner, EoH_MarkerState.OWNED, 0, GetGroupColor(owner));
        data.Pulse = 0;
        data.Normalize();
        EoH_MarkerService.Broadcast(data);
    }

    static void UpdateBaseTownMarker(string townName)
    {
        EoH_MarkerData data = BuildTownMarker(townName, "Unclaimed", EoH_MarkerState.NORMAL, 0, ARGB(120, 150, 150, 150));
        data.Pulse = 0;
        data.Normalize();
        EoH_MarkerService.Broadcast(data);
    }

    static void UpdateCapturingMarker(string townName, string owner)
    {
        EoH_MarkerData data = BuildTownMarker(townName, owner, EoH_MarkerState.CAPTURING, 1, GetGroupColor(owner));
        data.Label = townName + " Capturing";
        data.Icon = "Radio";
        data.Normalize();
        EoH_MarkerService.Broadcast(data);
    }

    static void UpdatePausedMarker(string townName, string owner)
    {
        EoH_MarkerData data = BuildTownMarker(townName, owner, EoH_MarkerState.NORMAL, 0, GetGroupColor(owner));
        data.Label = townName + " Capture Paused";
        data.Icon = "Territory";
        data.Pulse = 0;
        data.Normalize();
        EoH_MarkerService.Broadcast(data);
    }

    static void UpdateContestedMarker(string townName, string owner)
    {
        EoH_MarkerData data = BuildTownMarker(townName, owner, EoH_MarkerState.CONTESTED, 1, ARGB(255, 255, 50, 50));
        data.Label = townName + " Contested";
        data.Icon = "Exclamationmark";
        data.Normalize();
        EoH_MarkerService.Broadcast(data);
    }

    static void ClearActiveTownMarker(string townName)
    {
        string owner = EoH_CaptureManager.Get().GetTownOwner(townName);
        if (owner != "")
            UpdateTownMarker(townName, owner);
        else
            UpdateBaseTownMarker(townName);
    }

    static void ClearContestedMarker(string townName)
    {
        ClearActiveTownMarker(townName);
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
        int safePulse = 0;
        if (state == EoH_MarkerState.CAPTURING || state == EoH_MarkerState.CONTESTED)
            safePulse = pulse;

        vector markerPos = GetTownPosition(townName);

        EoH_MarkerData data = new EoH_MarkerData(GetMarkerId(townName), townName, markerPos);
        data.Category = EoH_MarkerCategory.TOWN;
        data.State = state;
        data.Owner = owner;
        data.Color = color;
        data.BaseColor = color;
        data.Pulse = safePulse;
        data.Is3D = 0;
        data.Icon = GetIconForState(state, owner);
        data.Visible = 1;
        data.Normalize();

        Print("[EoH_TownMarker] Build town=" + townName + " state=" + state + " pos=" + markerPos.ToString() + " icon=" + data.Icon);
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
        // Single source of truth: town markers must use the same relay position used by capture logic.
        vector pos = EoH_CaptureManager.Get().GetTownPos(townName);
        if (pos != "0 0 0".ToVector())
            return pos;

        EoH_CaptureTownConfig cfg = EoH_CaptureManager.Get().GetTownConfig(townName);
        if (cfg)
            return cfg.GetRelayVector();

        return "0 0 0".ToVector();
    }
};
