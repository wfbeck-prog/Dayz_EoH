class EoH_MarkerService
{
    protected static ref map<string, ref EoH_MarkerData> s_ServerMarkers = new map<string, ref EoH_MarkerData>();

    static void SendToPlayer(PlayerBase player, EoH_MarkerData data)
    {
        if (!player || !player.GetIdentity() || !data)
            return;

        data.Normalize();

        // Store authoritative server-side copy.
        s_ServerMarkers.Set(data.Id, data);

        Param1<ref EoH_MarkerData> param = new Param1<ref EoH_MarkerData>(data);
        GetGame().RPCSingleParam(player, EoH_MarkerRPC.ADD_OR_UPDATE_MARKER, param, true, player.GetIdentity());

        Print("[EoH_MarkerService] Sent server-controlled marker id=" + data.Id + " to=" + player.GetIdentity().GetName());
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
        s_ServerMarkers.Set(data.Id, data);

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

        Print("[EoH_MarkerService] Broadcast server-controlled marker id=" + data.Id + " label=" + data.Label);
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

        Print("[EoH_MarkerService] Removed server-controlled marker id=" + id);
    }

    static void ResendAllMarkersToPlayer(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        foreach (string id, EoH_MarkerData data : s_ServerMarkers)
        {
            if (!data)
                continue;

            Param1<ref EoH_MarkerData> param = new Param1<ref EoH_MarkerData>(data);
            GetGame().RPCSingleParam(player, EoH_MarkerRPC.ADD_OR_UPDATE_MARKER, param, true, player.GetIdentity());
        }

        Print("[EoH_MarkerService] Resent " + s_ServerMarkers.Count().ToString() + " server markers to " + player.GetIdentity().GetName());
    }
};
