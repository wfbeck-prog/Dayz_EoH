class EoH_QuestTravelOverlayConfig
{
    static ref array<ref EoH_QuestTravelOverlayData> s_Overlays;

    static void Init()
    {
        if (s_Overlays)
            return;

        s_Overlays = new array<ref EoH_QuestTravelOverlayData>();

        // Prototype: Signal and Ashes: Dead Air 1
        s_Overlays.Insert(new EoH_QuestTravelOverlayData(
            401001,
            501001,
            "Signal and Ashes: Search Area",
            "3060 310 7870".ToVector(),
            350.0
        ));

        Print("[EoH_TravelOverlay] Loaded prototype overlays=" + s_Overlays.Count().ToString());
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
};
