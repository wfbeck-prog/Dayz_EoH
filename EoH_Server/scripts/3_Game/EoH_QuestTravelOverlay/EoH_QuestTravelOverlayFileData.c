class EoH_QuestTravelOverlayFileEntry
{
    int QuestID;
    int ObjectiveID;
    string Label;

    // Enter the exact objective/task position here.
    // The mod automatically offsets the visible circle center so the target is inside the circle but not centered.
    vector TruePosition;

    float Radius;

    void EoH_QuestTravelOverlayFileEntry()
    {
        QuestID = 0;
        ObjectiveID = 0;
        Label = "";
        TruePosition = "0 0 0";
        Radius = 350.0;
    }
};

class EoH_QuestTravelOverlayFileConfig
{
    int ConfigVersion;
    ref array<ref EoH_QuestTravelOverlayFileEntry> Overlays;

    void EoH_QuestTravelOverlayFileConfig()
    {
        ConfigVersion = 2;
        Overlays = new array<ref EoH_QuestTravelOverlayFileEntry>();
    }
};
