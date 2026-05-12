class EoH_MarkerCategory
{
    static const string TOWN = "Town";
    static const string TRADER = "Trader";
    static const string KEYROOM = "KeyRoom";
    static const string CACHE = "Cache";
    static const string BUNKER = "Bunker";
    static const string INTEL = "Intel";
    static const string EVENT = "Event";
};

class EoH_MarkerState
{
    static const string NORMAL = "Normal";
    static const string ACTIVE = "Active";
    static const string CAPTURING = "Capturing";
    static const string CONTESTED = "Contested";
    static const string OWNED = "Owned";
    static const string HIDDEN = "Hidden";
};

class EoH_MarkerRPC
{
    // EOHM0 / EOHM1. Keep these unique to EoH_Server and do not reuse RoamingTrader RPC ids.
    static const int ADD_OR_UPDATE_MARKER = 0x454F484D;
    static const int REMOVE_MARKER = 0x454F484E;
};

class EoH_MarkerData
{
    string Id;
    string Label;
    vector Position;

    string Category;
    string State;
    string Owner;
    string Icon;

    int Color;
    int BaseColor;
    int Pulse;
    int Is3D;
    int Visible;

    void EoH_MarkerData(string id = "", string label = "", vector position = "0 0 0")
    {
        Id = id;
        Label = label;
        Position = position;

        Category = EoH_MarkerCategory.EVENT;
        State = EoH_MarkerState.NORMAL;
        Owner = "";
        Icon = "";

        Color = ARGB(255, 220, 220, 220);
        BaseColor = Color;
        Pulse = 0;
        Is3D = 1;
        Visible = 1;
    }

    void Normalize()
    {
        if (Id == "")
            Id = MakeSafeId(Category + "_" + Label);

        if (Label == "")
            Label = Id;

        if (Category == "")
            Category = EoH_MarkerCategory.EVENT;

        if (State == "")
            State = EoH_MarkerState.NORMAL;

        if (BaseColor == 0)
            BaseColor = Color;

        if (Color == 0)
            Color = BaseColor;

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
