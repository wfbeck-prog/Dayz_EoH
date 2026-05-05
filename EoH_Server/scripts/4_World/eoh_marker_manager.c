class EoH_MarkerManager
{
    static void SendToNearby(string id, vector pos, int rpcId, float radius = 1500)
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man m : players)
        {
            PlayerBase p = PlayerBase.Cast(m);
            if (!p || !p.GetIdentity()) continue;

            if (vector.Distance(p.GetPosition(), pos) > radius)
                continue;

            Param2<vector, string> data = new Param2<vector, string>(pos, id);

            GetGame().RPCSingleParam(p, rpcId, data, true, p.GetIdentity());
        }
    }
};
