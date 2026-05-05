modded class PlayerBase
{
    private float m_EoH_RT_NextRevealCheckTime;

    override void OnPlayerLoaded()
    {
        super.OnPlayerLoaded();
        m_EoH_RT_NextRevealCheckTime = 0;
    }

    override void EOnFrame(IEntity other, float timeSlice)
    {
        super.EOnFrame(other, timeSlice);

        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!IsAlive())
            return;

        float now = GetGame().GetTime();
        if (now < m_EoH_RT_NextRevealCheckTime)
            return;

        m_EoH_RT_NextRevealCheckTime = now + 1000;
        EoH_RT_CheckTraderRevealProximity();
    }

    void EoH_RT_CheckTraderRevealProximity()
    {
        EoH_RT_TraderManager manager = EoH_RT_TraderManager.Get();
        if (!manager)
            return;

        array<Object> objects = new array<Object>();
        array<CargoBase> proxy = new array<CargoBase>();

        GetGame().GetObjectsAtPosition3D(GetPosition(), 3.0, objects, proxy);

        foreach (Object obj : objects)
        {
            if (!obj)
                continue;

            EoH_RT_TraderRuntime runtime = manager.GetRuntimeByObject(obj);
            if (!runtime)
                continue;

            manager.RevealMarkerForObjectToAllPlayers(obj);
            return;
        }
    }
}
