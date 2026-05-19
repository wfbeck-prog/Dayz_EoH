class EoH_QuestTravelCircleManager
{
    private static ref EoH_QuestTravelCircleManager s_Instance;

    protected ref map<string, ref EoH_QuestTravelCircleData> m_Circles;

    void EoH_QuestTravelCircleManager()
    {
        m_Circles = new map<string, ref EoH_QuestTravelCircleData>();
    }

    static EoH_QuestTravelCircleManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_QuestTravelCircleManager();

        return s_Instance;
    }

    void Upsert(EoH_QuestTravelCircleData data)
    {
        if (!data)
            return;

        data.Normalize();
        m_Circles.Set(data.Id, data);

        Print("[EoH_QuestCircle] Upsert local circle id=" + data.Id + " radius=" + data.Radius.ToString());
    }

    void Remove(string id)
    {
        if (id == "")
            return;

        if (m_Circles.Contains(id))
            m_Circles.Remove(id);

        Print("[EoH_QuestCircle] Removed local circle id=" + id);
    }

    map<string, ref EoH_QuestTravelCircleData> GetAll()
    {
        return m_Circles;
    }
};
