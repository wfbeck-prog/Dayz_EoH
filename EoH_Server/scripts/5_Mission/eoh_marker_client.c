class EoH_ClientMarker
{
    string Id;
    vector Pos;
    string Name;
    int Color;
    int BaseColor;
    bool Pulse;
};

class EoH_ClientMarkers
{
    static ref map<string, ref EoH_ClientMarker> m_Markers;

    static void Add(string id, vector pos, string name, int color, bool pulse = false)
    {
        if (!m_Markers)
            m_Markers = new map<string, ref EoH_ClientMarker>();

        EoH_ClientMarker m;

        if (m_Markers.Contains(id))
        {
            m = m_Markers.Get(id);
        }
        else
        {
            m = new EoH_ClientMarker();
            m.Id = id;
            m_Markers.Set(id, m);
        }

        m.Pos = pos;
        m.Name = name;
        m.Color = color;
        m.BaseColor = color;
        m.Pulse = pulse;
    }

    static void Remove(string id)
    {
        if (!m_Markers) return;

        if (m_Markers.Contains(id))
            m_Markers.Remove(id);
    }

    static void UpdatePulse()
    {
        if (!m_Markers) return;

        int t = GetGame().GetTime() % 1000;

        foreach (string key, EoH_ClientMarker m : m_Markers)
        {
            if (!m.Pulse) continue;

            if (t < 500)
                m.Color = ARGB(255,255,50,50);
            else
                m.Color = ARGB(255,255,255,255);
        }
    }

    static void Render(MapWidget map)
    {
        if (!map || !m_Markers) return;

        UpdatePulse();

        foreach (string key, EoH_ClientMarker m : m_Markers)
        {
            map.AddUserMark(m.Pos, m.Name, m.Color, "\\dz\\gear\\navigation\\data\\map_tree_ca.paa");
        }
    }
};
