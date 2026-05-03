class EoH_TownMarkerRPC
{
    static const int ADD_OR_UPDATE_TOWN_MARKER = 0x454F4850;
    static const int REMOVE_TOWN_MARKER = 0x454F4851;
};

class EoH_TownMarkerData
{
    string MarkerId;
    string TownName;
    string OwnerGroupName;
    vector Position;
    int Color;
    int BaseColor;
    string Icon;
    int Is3D;
    int IsContested;
    int Pulse;

    void EoH_TownMarkerData(string markerId = "", string townName = "", string ownerGroupName = "", vector position = "0 0 0")
    {
        MarkerId = markerId;
        TownName = townName;
        OwnerGroupName = ownerGroupName;
        Position = position;
        Color = ARGB(255, 80, 180, 255);
        BaseColor = Color;
        Icon = "Flag";
        Is3D = 0;
        IsContested = 0;
        Pulse = 0;
    }

    string GetLabel()
    {
        if (IsContested == 1)
        {
            if (OwnerGroupName != "" && OwnerGroupName != "Unclaimed")
                return "CONTESTED: " + TownName + " - " + OwnerGroupName;

            return "CONTESTED: " + TownName;
        }

        if (OwnerGroupName != "" && OwnerGroupName != "Unclaimed")
            return TownName + " - " + OwnerGroupName;

        return TownName;
    }
};
