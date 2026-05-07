class EoH_BunkerPanelObserver
{
    protected static ref EoH_BunkerPanelObserver s_Instance;

    protected ref array<vector> m_PanelPositions;
    protected ref array<int> m_TriggeredPanels;
    protected float m_CheckTimer;
    protected bool m_Started;

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
        m_CheckTimer = 0;
        m_Started = false;

        // EoH bunker panel/loot-room area. Add more panel positions here if needed.
        m_PanelPositions.Insert("13267.886719 19.423756 6073.476563".ToVector());
    }

    void Start()
    {
        if (m_Started)
            return;

        m_Started = true;
        Print("[EoH_BunkerObserver] Started bunker panel observer");
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
                Print("[EoH_BunkerObserver] No underground panel found near " + pos.ToString());
                continue;
            }

            if (IsPanelOpen(panel))
            {
                Print("[EoH_BunkerObserver] Bunker panel opened near " + pos.ToString() + " type=" + panel.GetType());
                m_TriggeredPanels.Insert(i);
                EoH_BunkerGlobalAlert.Send(null);
            }
        }
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

    protected bool IsPanelOpen(Object panel)
    {
        if (!panel)
            return false;

        float phase = panel.GetAnimationPhase("Doors1");
        if (phase > 0.5)
            return true;

        phase = panel.GetAnimationPhase("Door1");
        if (phase > 0.5)
            return true;

        phase = panel.GetAnimationPhase("Panel");
        if (phase > 0.5)
            return true;

        phase = panel.GetAnimationPhase("Lid");
        if (phase > 0.5)
            return true;

        return false;
    }
}
