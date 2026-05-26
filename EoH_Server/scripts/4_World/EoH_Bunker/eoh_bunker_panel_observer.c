class EoH_BunkerPanelObserver
{
    protected static ref EoH_BunkerPanelObserver s_Instance;

    protected ref array<vector> m_PanelPositions;
    protected ref array<int> m_TriggeredPanels;
    protected ref array<string> m_LastPanelTypes;
    protected ref array<float> m_LastMissingPanelAdvisorTimes;
    protected float m_CheckTimer;
    protected bool m_Started;
    protected const float MISSING_PANEL_ADVISOR_INTERVAL = 300.0;

    static EoH_BunkerPanelObserver Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_BunkerPanelObserver();

        return s_Instance;
    }

    void EoH_BunkerPanelObserver()
    {
        m_PanelPositions = new array<vector>;
        m_TriggeredPanels = new array<int>;
        m_LastPanelTypes = new array<string>;
        m_LastMissingPanelAdvisorTimes = new array<float>;
        m_CheckTimer = 0;
        m_Started = false;

        // EoH bunker panel/loot-room area. Add more panel positions here if needed.
        m_PanelPositions.Insert("13267.886719 19.423756 6073.476563".ToVector());
        m_LastPanelTypes.Insert("");
        m_LastMissingPanelAdvisorTimes.Insert(0);
    }

    void Start()
    {
        if (m_Started)
            return;

        m_Started = true;
        Print("[EoH_BunkerObserver] Started bunker panel observer");
        EoH_LiveAdvisorLogger.Log("BUNKER_OBSERVER_START", "Bunker panel observer started", "info", "EoH_BunkerObserver");
    }

    void Update(float timeslice)
    {
        if (!m_Started)
            return;

        m_CheckTimer += timeslice;
        if (m_CheckTimer < 5.0)
            return;

        m_CheckTimer = 0;
        CheckPanels();
    }

    protected void CheckPanels()
    {
        for (int i = 0; i < m_PanelPositions.Count(); i++)
        {
            if (m_TriggeredPanels.Find(i) != -1)
                continue;

            vector pos = m_PanelPositions.Get(i);
            Object panel = FindPanelNear(pos, 8.0);

            if (!panel)
            {
                MaybeLogMissingPanel(i, pos);
                continue;
            }

            string panelType = panel.GetType();
            if (i < m_LastPanelTypes.Count() && m_LastPanelTypes.Get(i) != panelType)
            {
                m_LastPanelTypes.Set(i, panelType);
                string foundMessage = "Found bunker panel candidate near " + pos.ToString() + " type=" + panelType + " at " + panel.GetPosition().ToString();
                Print("[EoH_BunkerObserver] " + foundMessage);
                EoH_LiveAdvisorLogger.Log("BUNKER_PANEL_FOUND", foundMessage, "info", "EoH_BunkerObserver");
            }

            // Safe fallback: this observer now confirms the actual panel object/class without using unsupported Object animation APIs.
            // The action bridge remains the primary open-event hook. If action bridge still does not fire, use the logged class/type
            // here to build a type-specific observer against the real panel class.
        }
    }

    protected void MaybeLogMissingPanel(int index, vector pos)
    {
        float now = GetGame().GetTime() / 1000.0;

        while (m_LastMissingPanelAdvisorTimes.Count() <= index)
        {
            m_LastMissingPanelAdvisorTimes.Insert(0);
        }

        float lastReport = m_LastMissingPanelAdvisorTimes.Get(index);

        if (lastReport > 0 && now - lastReport < MISSING_PANEL_ADVISOR_INTERVAL)
        {
            return;
        }

        m_LastMissingPanelAdvisorTimes.Set(index, now);

        string message = "No underground panel found near " + pos.ToString() + ". Check bunker panel coordinates or object class.";
        Print("[EoH_BunkerObserver] " + message);
        EoH_LiveAdvisorLogger.Log("BUNKER_PANEL_MISSING", message, "warning", "EoH_BunkerObserver");
    }

    protected Object FindPanelNear(vector pos, float radius)
    {
        array<Object> objects = new array<Object>;
        array<CargoBase> cargo = new array<CargoBase>;
        GetGame().GetObjectsAtPosition(pos, radius, objects, cargo);

        foreach (Object obj : objects)
        {
            if (!obj)
                continue;

            string type = obj.GetType();
            if (type == "Land_Underground_Panel")
                return obj;

            if (type.Contains("Underground") && type.Contains("Panel"))
                return obj;
        }

        return null;
    }
}
