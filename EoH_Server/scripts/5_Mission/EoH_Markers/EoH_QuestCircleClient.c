class EoH_QuestCircleClient
{
    protected static ref map<string, ref EoH_QuestCircleData> s_Circles = new map<string, ref EoH_QuestCircleData>();

    static void AddOrUpdateCircle(EoH_QuestCircleData data)
    {
        if (!data)
            return;

        data.Normalize();
        s_Circles.Set(data.Id, data);
        Print("[EoH_QuestCircleClient] Added circle id=" + data.Id + " radius=" + data.Radius.ToString());
    }

    static void RemoveCircle(string id)
    {
        if (id == "")
            return;

        if (s_Circles.Contains(id))
            s_Circles.Remove(id);

        Print("[EoH_QuestCircleClient] Removed circle id=" + id);
    }

    static map<string, ref EoH_QuestCircleData> GetCircles()
    {
        return s_Circles;
    }
};

modded class MissionGameplay
{
    override void OnEvent(EventType eventTypeId, Param params)
    {
        super.OnEvent(eventTypeId, params);

        if (eventTypeId != ChatMessageEventTypeID)
            return;
    }
};
