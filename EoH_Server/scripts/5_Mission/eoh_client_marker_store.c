class EoH_ClientMarkerStore
{
    protected static ref map<string, ref EoH_TownMarkerData> s_Markers;

    static void Ensure()
    {
        if (!s_Markers)
            s_Markers = new map<string, ref EoH_TownMarkerData>();
    }

    static void AddOrUpdate(EoH_TownMarkerData data)
    {
        if (!data)
            return;

        Ensure();
        data.Normalize();

        if (data.Id == "")
            return;

        s_Markers.Set(data.Id, data);
        Print("[EoH_ClientMarkers] Stored marker: " + data.Id + " / " + data.GetLabel());
    }

    static void Remove(string id)
    {
        Ensure();

        if (s_Markers.Contains(id))
        {
            s_Markers.Remove(id);
            Print("[EoH_ClientMarkers] Removed marker: " + id);
        }
    }

    static void ApplyToMap(MapWidget mapWidget)
    {
        if (!mapWidget)
            return;

        Ensure();

        foreach (string id, EoH_TownMarkerData data : s_Markers)
        {
            if (!data)
                continue;

            data.Normalize();

            string label = data.GetLabel();
            int color = data.Color;
            string icon = "\\dz\\gear\\navigation\\data\\map_tree_ca.paa";

            mapWidget.AddUserMark(data.Position, label, color, icon);
        }

        Print("[EoH_ClientMarkers] Applied markers to map: " + s_Markers.Count().ToString());
    }
};
