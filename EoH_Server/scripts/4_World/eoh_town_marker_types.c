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
    string Icon;
    int Is3D;

    void EoH_TownMarkerData(string markerId = "", string townName = "", string ownerGroupName = "", vector position = "0 0 0")
    {
        MarkerId = markerId;
        TownName = townName;
        OwnerGroupName = ownerGroupName;
        Position = position;
        Color = ARGB(255, 80, 180, 255);
        Icon = "Flag";
        Is3D = 0;
    }

    string GetLabel()
    {
        if (OwnerGroupName != "" && OwnerGroupName != "Unclaimed")
            return TownName + " - " + OwnerGroupName;

        return TownName;
    }
};
