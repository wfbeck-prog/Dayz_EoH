class EoH_MarkerService
{
    static void SendToPlayer(PlayerBase player, EoH_MarkerData data)
    {
        if (!player || !player.GetIdentity() || !data)
            return;

        data.Normalize();

        Param1<ref EoH_MarkerData> param = new Param1<ref EoH_MarkerData>(data);
        GetGame().RPCSingleParam(player, EoH_MarkerRPC.ADD_OR_UPDATE_MARKER, param, true, player.GetIdentity());
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

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man m : players)
        {
            PlayerBase player = PlayerBase.Cast(m);
            if (!player)
                continue;

            SendToPlayer(player, data);
        }
    }

    static void RemoveFromAll(string id)
    {
        if (id == "")
            return;

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man m : players)
        {
            PlayerBase player = PlayerBase.Cast(m);
            if (!player)
                continue;

            RemoveFromPlayer(player, id);
        }
    }
};
