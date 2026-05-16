class EoH_MarkerService
{
    protected static ref map<string, ref EoH_MarkerData> s_ServerMarkers = new map<string, ref EoH_MarkerData>();

    static bool IsQuestMarker(EoH_MarkerData data)
    {
        if (!data)
            return false;

        string category = data.Category;
        category.ToLower();
        string id = data.Id;
        id.ToLower();
        string label = data.Label;
        label.ToLower();

        return category.Contains("quest") || id.Contains("quest") || label.Contains("quest");
    }

    static string ResolveExpansionIcon(EoH_MarkerData data)
    {
        if (!data || data.Icon == "")
            return "MapMarker";

        return data.Icon;
    }

    static bool CreateOrUpdateExpansionServerMarker(EoH_MarkerData data)
    {
        if (!data)
            return false;

        ExpansionMarkerModule markerModule = ExpansionMarkerModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarkerModule));
        if (!markerModule)
        {
            Print("[EoH_MarkerService][WARN] ExpansionMarkerModule unavailable for server marker id=" + data.Id);
            return false;
        }

        markerModule.CreateServerMarker(data.Label, ResolveExpansionIcon(data), data.Position, data.Color, data.Is3D == 1, data.Id);
        return true;
    }

    static void SendToPlayer(PlayerBase player, EoH_MarkerData data)
    {
        if (!player || !player.GetIdentity() || !data)
            return;

        data.Normalize();

        if (!IsQuestMarker(data))
        {
            s_ServerMarkers.Set(data.Id, data);
            CreateOrUpdateExpansionServerMarker(data);
            Print("[EoH_MarkerService] Created Expansion SERVER marker id=" + data.Id + " label=" + data.Label);
            return;
        }

        Param1<ref EoH_MarkerData> param = new Param1<ref EoH_MarkerData>(data);
        GetGame().RPCSingleParam(player, EoH_MarkerRPC.ADD_OR_UPDATE_MARKER, param, true, player.GetIdentity());

        Print("[EoH_MarkerService] Sent personal QUEST marker id=" + data.Id + " to=" + player.GetIdentity().GetName());
    }

    static void RemoveFromPlayer(PlayerBase player, string id)
    {
        if (!player || !player.GetIdentity() || id == "")
            return;

        Param1<string> param = new Param1<string>(id);
        GetGame().RPCSingleParam(player, EoH_MarkerRPC.REMOVE_MARKER, param, true, player.GetIdentity());
    }

    static void Broadcast(EoH_MarkerData data)
    {
        if (!data)
            return;

        data.Normalize();

        if (!IsQuestMarker(data))
        {
            s_ServerMarkers.Set(data.Id, data);
            CreateOrUpdateExpansionServerMarker(data);
            Print("[EoH_MarkerService] Broadcast Expansion SERVER marker id=" + data.Id + " label=" + data.Label);
            return;
        }

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man m : players)
        {
            PlayerBase player = PlayerBase.Cast(m);
            if (!player)
                continue;

            Param1<ref EoH_MarkerData> param = new Param1<ref EoH_MarkerData>(data);
            GetGame().RPCSingleParam(player, EoH_MarkerRPC.ADD_OR_UPDATE_MARKER, param, true, player.GetIdentity());
        }

        Print("[EoH_MarkerService] Broadcast personal QUEST marker id=" + data.Id + " label=" + data.Label);
    }

    static void RemoveFromAll(string id)
    {
        if (id == "")
            return;

        if (s_ServerMarkers.Contains(id))
            s_ServerMarkers.Remove(id);

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man m : players)
        {
            PlayerBase player = PlayerBase.Cast(m);
            if (!player)
                continue;

            RemoveFromPlayer(player, id);
        }

        Print("[EoH_MarkerService] Removed EoH marker id=" + id);
    }

    static void ResendAllMarkersToPlayer(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        foreach (string id, EoH_MarkerData data : s_ServerMarkers)
        {
            if (!data)
                continue;

            CreateOrUpdateExpansionServerMarker(data);
        }

        Print("[EoH_MarkerService] Refreshed " + s_ServerMarkers.Count().ToString() + " Expansion server markers for " + player.GetIdentity().GetName());
    }
};
