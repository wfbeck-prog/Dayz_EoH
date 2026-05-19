class EoH_QuestTravelOverlayFileEntry
{
    int QuestID;
    int ObjectiveID;
    string Label;
    vector Position;
    float Radius;

    void EoH_QuestTravelOverlayFileEntry()
    {
        QuestID = 0;
        ObjectiveID = 0;
        Label = "";
        Position = "0 0 0";
        Radius = 350.0;
    }
};

class EoH_QuestTravelOverlayFileConfig
{
    int ConfigVersion;
    ref array<ref EoH_QuestTravelOverlayFileEntry> Overlays;

    void EoH_QuestTravelOverlayFileConfig()
    {
        ConfigVersion = 1;
        Overlays = new array<ref EoH_QuestTravelOverlayFileEntry>();
    }
};
