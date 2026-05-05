class EoH_MarkerManager
{
    private static ref EoH_MarkerManager s_Instance;

    protected ref map<string, ref EoH_MarkerData> m_Markers;
    protected float m_PulseTime;

    void EoH_MarkerManager()
    {
        m_Markers = new map<string, ref EoH_MarkerData>();
        m_PulseTime = 0;

        if (!GetGame().IsDedicatedServer())
            GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(OnUpdate);
    }

    void ~EoH_MarkerManager()
    {
        if (!GetGame().IsDedicatedServer())
            GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Remove(OnUpdate);
    }

    static EoH_MarkerManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_MarkerManager();

        return s_Instance;
    }

    void Upsert(EoH_MarkerData data)
    {
        if (!data)
            return;

        data.Normalize();

        m_Markers.Set(data.Id, data);
        ApplyMarker(data, false);
    }

    void Remove(string id)
    {
        if (id == "")
            return;

        m_Markers.Remove(id);

        ExpansionMarkerModule markerModule;
        CF_Modules<ExpansionMarkerModule>.Get(markerModule);

        if (markerModule)
            markerModule.RemovePersonalMarkerByUID(id);
    }

    protected void ApplyMarker(EoH_MarkerData data, bool pulse)
    {
        if (!data || data.Visible == 0)
            return;

        ExpansionMarkerModule markerModule;
        CF_Modules<ExpansionMarkerModule>.Get(markerModule);

        if (!markerModule || !markerModule.GetData())
            return;

        ExpansionMarkerData marker = markerModule.GetData().PersonalGetByUID(data.Id);

        if (!marker)
        {
            marker = ExpansionMarkerData.Create(ExpansionMapMarkerType.PERSONAL, data.Id);
            marker.Set3D(data.Is3D == 1);
            marker.SetIcon(data.Icon);
            markerModule.CreateMarker(marker);
        }

        marker.SetName(data.Label);
        marker.SetPosition(data.Position);
        marker.SetIcon(data.Icon);
        marker.Set3D(data.Is3D == 1);

        int color = data.BaseColor;

        if (pulse && data.Pulse == 1)
        {
            float wave = Math.AbsFloat(Math.Sin(m_PulseTime * 5.0));
            if (wave > 0.5)
                color = ARGB(255, 255, 255, 255);
        }
        else
        {
            color = data.Color;
        }

        marker.SetColor(color);
        markerModule.Refresh();
    }

    void OnUpdate(float timeslice)
    {
        m_PulseTime += timeslice;

        if (!m_Markers || m_Markers.Count() == 0)
            return;

        for (int i = 0; i < m_Markers.Count(); i++)
        {
            EoH_MarkerData data = m_Markers.GetElement(i);
            if (!data)
                continue;

            if (data.Pulse != 1)
                continue;

            ApplyMarker(data, true);
        }
    }
};
