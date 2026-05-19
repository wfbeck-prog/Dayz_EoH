modded class ExpansionMapMenu
{
    protected CanvasWidget m_EoH_QuestCircleCanvas;
    protected MapWidget m_EoH_MapWidget;

    override Widget Init()
    {
        Widget root = super.Init();
        EoH_CreateQuestCircleCanvas(root);
        return root;
    }

    override void Update(float timeslice)
    {
        super.Update(timeslice);
        EoH_DrawQuestTravelCircles();
    }

    void EoH_CreateQuestCircleCanvas(Widget root)
    {
        if (!root || m_EoH_QuestCircleCanvas)
            return;

        Widget mapWidget = root.FindAnyWidget("Map");
        if (!mapWidget)
            mapWidget = root.FindAnyWidget("map");

        m_EoH_MapWidget = MapWidget.Cast(mapWidget);

        Widget parent = mapWidget;
        if (!parent)
            parent = root;

        m_EoH_QuestCircleCanvas = CanvasWidget.Cast(GetGame().GetWorkspace().CreateWidgets("EoH_Server/scripts/5_Mission/EoH_QuestTravelOverlay/layouts/eoh_quest_circle_canvas.layout", parent));

        if (m_EoH_QuestCircleCanvas)
        {
            m_EoH_QuestCircleCanvas.SetSort(9999);
            m_EoH_QuestCircleCanvas.Show(true);
            Print("[EoH_QuestCircle] Created Expansion map circle canvas.");
        }
        else
        {
            Print("[EoH_QuestCircle][WARN] Failed to create Expansion map circle canvas.");
        }

        if (!m_EoH_MapWidget)
            Print("[EoH_QuestCircle][WARN] Could not find MapWidget by layout name Map/map.");
    }

    void EoH_DrawQuestTravelCircles()
    {
        if (!m_EoH_QuestCircleCanvas || !m_EoH_MapWidget)
            return;

        m_EoH_QuestCircleCanvas.Clear();

        map<string, ref EoH_QuestTravelCircleData> circles = EoH_QuestTravelCircleManager.Get().GetAll();
        if (!circles || circles.Count() == 0)
            return;

        foreach (string id, EoH_QuestTravelCircleData circle : circles)
        {
            if (!circle || circle.Visible == 0)
                continue;

            EoH_DrawCircleOnMap(m_EoH_MapWidget, circle);
        }
    }

    void EoH_DrawCircleOnMap(MapWidget mapWidget, EoH_QuestTravelCircleData circle)
    {
        if (!mapWidget || !m_EoH_QuestCircleCanvas || !circle)
            return;

        int segments = 72;
        float step = Math.PI2 / segments;

        vector prevWorld;
        vector nextWorld;
        float prevX;
        float prevY;
        float nextX;
        float nextY;

        for (int i = 0; i <= segments; i++)
        {
            float angleA = (i - 1) * step;
            float angleB = i * step;

            prevWorld = circle.Center;
            nextWorld = circle.Center;

            prevWorld[0] = circle.Center[0] + Math.Cos(angleA) * circle.Radius;
            prevWorld[2] = circle.Center[2] + Math.Sin(angleA) * circle.Radius;

            nextWorld[0] = circle.Center[0] + Math.Cos(angleB) * circle.Radius;
            nextWorld[2] = circle.Center[2] + Math.Sin(angleB) * circle.Radius;

            mapWidget.MapToScreen(prevWorld, prevX, prevY);
            mapWidget.MapToScreen(nextWorld, nextX, nextY);

            m_EoH_QuestCircleCanvas.DrawLine(prevX, prevY, nextX, nextY, 2, circle.Color);
        }
    }
};
