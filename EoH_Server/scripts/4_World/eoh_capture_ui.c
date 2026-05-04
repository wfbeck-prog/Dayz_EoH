class EoH_CaptureUI
{
    static void Send(PlayerBase player, string msg)
    {
        if (!player || !player.GetIdentity()) return;
        GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>(msg), true, player.GetIdentity());
    }

    static void Broadcast(string msg)
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        foreach (Man m : players)
        {
            PlayerBase p = PlayerBase.Cast(m);
            if (p && p.GetIdentity())
                Send(p, msg);
        }
    }
};
