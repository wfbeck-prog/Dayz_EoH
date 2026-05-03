class EoH_TownMarkerData
{
    string Id;
    string MarkerId;
    string Name;
    string TownName;
    string Owner;
    string OwnerGroupName;
    vector Position;
    string Icon;
    int Color;
    int BaseColor;
    int IsContested;
    int Pulse;

    void EoH_TownMarkerData()
    {
        Id = "";
        MarkerId = "";
        Name = "";
        TownName = "";
        Owner = "";
        OwnerGroupName = "";
        Position = "0 0 0";
        Icon = "Territory";
        Color = ARGB(120, 150, 150, 150);
        BaseColor = Color;
        IsContested = 0;
        Pulse = 0;
    }

    void Normalize()
    {
        if (MarkerId == "")
            MarkerId = Id;

        if (Id == "")
            Id = MarkerId;

        if (TownName == "")
            TownName = Name;

        if (Name == "")
            Name = TownName;

        if (OwnerGroupName == "")
            OwnerGroupName = Owner;

        if (Owner == "")
            Owner = OwnerGroupName;

        if (Icon == "")
            Icon = "Territory";
    }

    string GetLabel()
    {
        Normalize();

        if (IsContested == 1)
            return Name + " - CONTESTED";

        if (Owner != "" && Owner != "Unclaimed")
            return Name + " - " + Owner;

        return Name;
    }
};
