class EoH_QuestTravelOverlayConfig
{
    static ref array<ref EoH_QuestTravelOverlayData> s_Overlays;

    static void Init()
    {
        if (s_Overlays)
            return;

        s_Overlays = new array<ref EoH_QuestTravelOverlayData>();

        // Prototype: Signal and Ashes: Dead Air 1
        EoH_QuestTravelOverlayData deadAir = new EoH_QuestTravelOverlayData();
        deadAir.QuestID = 401001;
        deadAir.ObjectiveID = 501001;
        deadAir.Label = "Signal and Ashes: Search Area";
        deadAir.TruePosition = "3060 310 7870".ToVector();
        deadAir.Radius = 350.0;
        deadAir.CircleCenter = EoH_QuestTravelOverlayData.ComputeOffsetCenter(deadAir.TruePosition, deadAir.QuestID, deadAir.ObjectiveID, deadAir.Radius);
        s_Overlays.Insert(deadAir);

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
