class EoH_QuestCircleRPC
{
    static const int ADD_OR_UPDATE_CIRCLE = 0x454F5143; // EOQC
    static const int REMOVE_CIRCLE = 0x454F5144;        // EOQD
};

class EoH_QuestCircleData
{
    string Id;
    string Label;
    vector Position;
    float Radius;
    int Color;
    int FillColor;
    int Visible;

    void EoH_QuestCircleData(string id = "", string label = "", vector position = "0 0 0", float radius = 500.0)
    {
        Id = id;
        Label = label;
        Position = position;
        Radius = radius;
        Color = ARGB(220, 255, 220, 80);
        FillColor = ARGB(45, 255, 220, 80);
        Visible = 1;
    }

    void Normalize()
    {
        if (Id == "")
            Id = MakeSafeId("QuestCircle_" + Label);

        if (Label == "")
            Label = Id;

        if (Radius <= 0)
            Radius = 500.0;

        if (Visible != 0)
            Visible = 1;
    }

    static string MakeSafeId(string raw)
    {
        string id = raw;
        id.Replace(" ", "_");
        id.Replace("/", "_");
        id.Replace("\\", "_");
        id.Replace(":", "_");
        return id;
    }
};
