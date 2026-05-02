modded class MapMenu extends UIScriptedMenu
{
	protected MapWidget m_EoH_RT_MapWidget;

	override Widget Init()
	{
		Widget root = super.Init();
		m_EoH_RT_MapWidget = EoH_RT_FindMapWidget(root);
		return root;
	}

	override void OnShow()
	{
		super.OnShow();
		if (!m_EoH_RT_MapWidget)
			m_EoH_RT_MapWidget = EoH_RT_FindMapWidget(layoutRoot);
		EoH_RT_RedrawMarkers();
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);
		EoH_RT_RedrawMarkers();
	}

	protected MapWidget EoH_RT_FindMapWidget(Widget w)
	{
		if (!w)
			return NULL;
		MapWidget mapWidget = MapWidget.Cast(w);
		if (mapWidget)
			return mapWidget;
		mapWidget = EoH_RT_FindMapWidget(w.GetChildren());
		if (mapWidget)
			return mapWidget;
		return EoH_RT_FindMapWidget(w.GetSibling());
	}

	protected void EoH_RT_RedrawMarkers()
	{
		if (!m_EoH_RT_MapWidget)
			return;

		map<string, ref EoH_RT_MarkerData> markers = EoH_RT_MarkerManager.GetInstance().GetMarkers();
		if (!markers)
			return;

		m_EoH_RT_MapWidget.ClearUserMarks();
		for (int i = 0; i < markers.Count(); i++)
		{
			EoH_RT_MarkerData data = markers.GetElement(i);
			if (!data)
				continue;
			m_EoH_RT_MapWidget.AddUserMark(data.Position, data.Label, ARGB(255, 220, 20, 60), "");
		}
	}
};
