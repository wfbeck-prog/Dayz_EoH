class EoH_RT_MarkerManager
{
	private static ref EoH_RT_MarkerManager s_Instance;
	protected ref map<string, ref EoH_RT_MarkerData> m_Markers;
	protected float m_PulseTime;

	void EoH_RT_MarkerManager()
	{
		m_Markers = new map<string, ref EoH_RT_MarkerData>();
		m_PulseTime = 0;

		if (!GetGame().IsDedicatedServer())
			GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(OnUpdate);
	}

	void ~EoH_RT_MarkerManager()
	{
		if (!GetGame().IsDedicatedServer())
			GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Remove(OnUpdate);
	}

	static EoH_RT_MarkerManager GetInstance()
	{
		if (!s_Instance)
			s_Instance = new EoH_RT_MarkerManager();

		return s_Instance;
	}

	void SetMarker(EoH_RT_MarkerData data)
	{
		if (!data || data.TraderId == "")
			return;

		m_Markers.Set(data.TraderId, data);
		ApplyMarker(data, false);
	}

	void RemoveMarker(string traderId)
	{
		if (traderId == "")
			return;

		m_Markers.Remove(traderId);

		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);

		if (markerModule)
			markerModule.RemovePersonalMarkerByUID(traderId);
	}

	void ApplyMarker(EoH_RT_MarkerData data, bool pulseColor)
	{
		if (!data)
			return;

		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);

		if (!markerModule || !markerModule.GetData())
			return;

		ExpansionMarkerData marker = markerModule.GetData().PersonalGetByUID(data.TraderId);

		if (!marker)
		{
			marker = ExpansionMarkerData.Create(ExpansionMapMarkerType.PERSONAL, data.TraderId);
			marker.Set3D(data.Is3D == 1);
			marker.SetIcon(data.Icon);
			markerModule.CreateMarker(marker);
		}

		marker.SetName(data.Label);
		marker.SetPosition(data.Position);
		marker.SetIcon(data.Icon);
		marker.Set3D(data.Is3D == 1);

		int color = data.Color;

		if (pulseColor && data.Pulse == 1 && data.TraderId.IndexOf("KR_") == 0)
		{
			float wave = Math.AbsFloat(Math.Sin(m_PulseTime * 5.0));

			if (wave > 0.5)
				color = ARGB(255, 255, 255, 255);
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
			EoH_RT_MarkerData data = m_Markers.GetElement(i);

			if (!data)
				continue;

			// Only KeyRoom markers flash. Trader markers stay independent.
			if (data.TraderId.IndexOf("KR_") != 0)
				continue;

			if (data.Pulse != 1)
				continue;

			ApplyMarker(data, true);
		}
	}

	map<string, ref EoH_RT_MarkerData> GetMarkers()
	{
		return m_Markers;
	}
};