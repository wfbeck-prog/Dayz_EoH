class EoH_QuestCircleService
{
    protected static ref map<string, ref EoH_QuestCircleData> s_Circles = new map<string, ref EoH_QuestCircleData>();

    static void SendToPlayer(PlayerBase player, EoH_QuestCircleData data)
    {
        if (!player || !player.GetIdentity() || !data)
            return;

        data.Normalize();
        s_Circles.Set(data.Id, data);

        Param1<ref EoH_QuestCircleData> param = new Param1<ref EoH_QuestCircleData>(data);
        GetGame().RPCSingleParam(player, EoH_QuestCircleRPC.ADD_OR_UPDATE_CIRCLE, param, true, player.GetIdentity());

        Print("[EoH_QuestCircle] Sent circle id=" + data.Id + " radius=" + data.Radius.ToString());
    }

    static void RemoveFromPlayer(PlayerBase player, string id)
    {
        if (!player || !player.GetIdentity() || id == "")
            return;

        Param1<string> param = new Param1<string>(id);
        GetGame().RPCSingleParam(player, EoH_QuestCircleRPC.REMOVE_CIRCLE, param, true, player.GetIdentity());
    }

    static void ResendToPlayer(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        foreach (string id, EoH_QuestCircleData data : s_Circles)
        {
            SendToPlayer(player, data);
        }
    }
};
