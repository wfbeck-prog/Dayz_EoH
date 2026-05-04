class EoH_ClientMarker
{
    vector Pos;
    string Name;
    int Color;
    int BaseColor;
    bool Pulse;
};

class EoH_ClientMarkers
{
    static ref array<ref EoH_ClientMarker> m_Markers;

    static void Add(vector pos, string name, int color, bool pulse = false)
    {
        if (!m_Markers)
            m_Markers = new array<ref EoH_ClientMarker>();

        EoH_ClientMarker m = new EoH_ClientMarker();
        m.Pos = pos;
        m.Name = name;
        m.Color = color;
        m.BaseColor = color;
        m.Pulse = pulse;

        m_Markers.Insert(m);
    }

    static void Clear()
    {
        if (m_Markers)
            m_Markers.Clear();
    }

    static void UpdatePulse()
    {
        if (!m_Markers)
            return;

        int t = GetGame().GetTime() % 1000;

        foreach (EoH_ClientMarker m : m_Markers)
        {
            if (!m.Pulse)
                continue;

            if (t < 500)
                m.Color = ARGB(255, 255, 50, 50);
            else
                m.Color = ARGB(255, 255, 255, 255);
        }
    }

    static void Render(MapWidget map)
    {
        if (!map || !m_Markers)
            return;

        UpdatePulse();

        foreach (EoH_ClientMarker m : m_Markers)
        {
            map.AddUserMark(
                m.Pos,
                m.Name,
                m.Color,
                "\\dz\\gear\\navigation\\data\\map_tree_ca.paa"
            );
        }
    }
};
