class EoH_QuestTravelCircleRPC
{
    // EOHQ0 / EOHQ1. Personal quest circle overlay RPCs.
    static const int ADD_OR_UPDATE_CIRCLE = 0x454F4851;
    static const int REMOVE_CIRCLE = 0x454F4852;
};

class EoH_QuestTravelCircleData
{
    string Id;
    string Label;
    vector Center;
    float Radius;
    int Color;
    int Visible;

    void EoH_QuestTravelCircleData(string id = "", string label = "", vector center = "0 0 0", float radius = 350.0)
    {
        Id = id;
        Label = label;
        Center = center;
        Radius = radius;
        Color = ARGB(180, 255, 220, 80);
        Visible = 1;
    }

    void Normalize()
    {
        if (Id == "")
            Id = "EoH_QUEST_CIRCLE_" + Label;

        if (Label == "")
            Label = Id;

        if (Radius < 25.0)
            Radius = 25.0;

        if (Color == 0)
            Color = ARGB(180, 255, 220, 80);

        if (Visible != 0)
            Visible = 1;
    }
};
