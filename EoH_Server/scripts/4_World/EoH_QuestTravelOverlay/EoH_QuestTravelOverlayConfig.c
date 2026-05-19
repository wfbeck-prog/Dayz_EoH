class EoH_QuestTravelOverlayConfig
{
    static ref array<ref EoH_QuestTravelOverlayData> s_Overlays;

    static void Init()
    {
        if (s_Overlays)
            return;

        s_Overlays = new array<ref EoH_QuestTravelOverlayData>();

        // Signal and Ashes: Dead Air 1
        AddOverlay(401001, 501001, "Signal and Ashes: Search Area", "3060 310 7870".ToVector(), 350.0);

        Print("[EoH_TravelOverlay] Loaded quest travel overlays=" + s_Overlays.Count().ToString());
    }

    static void AddOverlay(int questId, int objectiveId, string label, vector truePosition, float radius)
    {
        EoH_QuestTravelOverlayData data = new EoH_QuestTravelOverlayData(questId, objectiveId, label, truePosition, radius);
        s_Overlays.Insert(data);
        Print("[EoH_TravelOverlay] Config overlay quest=" + questId.ToString() + " objective=" + objectiveId.ToString() + " label=" + label + " center=" + data.CircleCenter.ToString() + " radius=" + radius.ToString());
    }

    static array<ref EoH_QuestTravelOverlayData> GetAll()
    {
        Init();
        return s_Overlays;
    }

    static EoH_QuestTravelOverlayData GetByObjective(int objectiveId)
    {
        Init();

        foreach (EoH_QuestTravelOverlayData data : s_Overlays)
        {
            if (data && data.ObjectiveID == objectiveId)
                return data;
        }

        return null;
    }

    static bool HasQuest(int questId)
    {
        Init();

        foreach (EoH_QuestTravelOverlayData data : s_Overlays)
        {
            if (data && data.QuestID == questId)
                return true;
        }

        return false;
    }
};
