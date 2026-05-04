class EoH_ClientMarkers
{
    static ref array<ref Param3<vector, string, int>> m_Markers;

    static void Add(vector pos, string name, int color)
    {
        if (!m_Markers)
            m_Markers = new array<ref Param3<vector, string, int>>();

        m_Markers.Insert(new Param3<vector, string, int>(pos, name, color));
    }

    static void Clear()
    {
        if (m_Markers)
            m_Markers.Clear();
    }

    static void Render(MapWidget map)
    {
        if (!map || !m_Markers)
            return;

        foreach (Param3<vector, string, int> m : m_Markers)
        {
            map.AddUserMark(
                m.param1,
                m.param2,
                m.param3,
                "\\dz\\gear\\navigation\\data\\map_tree_ca.paa"
            );
        }
    }
};
