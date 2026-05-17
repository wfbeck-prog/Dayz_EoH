class EoH_QuestTravelOverlayClient
{
    protected static ref map<string, ref EoH_QuestCircleData> s_TravelCircles = new map<string, ref EoH_QuestCircleData>();

    static void AddCircle(EoH_QuestCircleData circle)
    {
        if (!circle)
            return;

        circle.Normalize();
        s_TravelCircles.Set(circle.Id, circle);
        Print("[EoH_TravelOverlayClient] Added circle id=" + circle.Id + " label=" + circle.Label + " pos=" + circle.Position.ToString() + " radius=" + circle.Radius.ToString());
    }

    static void RemoveCircle(string id)
    {
        if (id == "")
            return;

        if (s_TravelCircles.Contains(id))
            s_TravelCircles.Remove(id);

        Print("[EoH_TravelOverlayClient] Removed circle id=" + id);
    }

    static map<string, ref EoH_QuestCircleData> GetCircles()
    {
        return s_TravelCircles;
    }
};

modded class MissionGameplay
{
    override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, target, rpc_type, ctx);

        if (rpc_type == EoH_QuestCircleRPC.ADD_OR_UPDATE_CIRCLE)
        {
            Param1<ref EoH_QuestCircleData> addParam;
            if (ctx.Read(addParam) && addParam && addParam.param1)
                EoH_QuestTravelOverlayClient.AddCircle(addParam.param1);
            return;
        }

        if (rpc_type == EoH_QuestCircleRPC.REMOVE_CIRCLE)
        {
            Param1<string> removeParam;
            if (ctx.Read(removeParam) && removeParam)
                EoH_QuestTravelOverlayClient.RemoveCircle(removeParam.param1);
            return;
        }
    }
};
