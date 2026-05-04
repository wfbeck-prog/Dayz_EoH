modded class MapMenu
{
    override Widget Init()
    {
        Widget root = super.Init();

        if (m_MapWidgetInstance)
        {
            EoH_ClientMarkers.Render(m_MapWidgetInstance);
        }

        return root;
    }
};
