class EoH_RewardSmokeMarker extends House
{
    protected Object m_EoH_SmokeObject;
    protected int m_EoH_SmokeAttempts;

    void EoH_RewardSmokeMarker()
    {
        m_EoH_SmokeAttempts = 0;
        SetEventMask(EntityEvent.INIT);
    }

    override void EOnInit(IEntity other, int extra)
    {
        super.EOnInit(other, extra);
        EoH_ScheduleSmokeStart();
    }

    void EoH_ScheduleSmokeStart()
    {
        if (!GetGame())
            return;

        if (!GetGame().IsServer())
            return;

        Print("[EoH_TownReward][SMOKE] Server init reward smoke marker pos=" + GetPosition().ToString());

        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(EoH_StartSmoke, 100, false);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(EoH_StartSmoke, 500, false);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(EoH_StartSmoke, 1500, false);
    }

    void EoH_StartSmoke()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        m_EoH_SmokeAttempts++;

        if (m_EoH_SmokeObject)
            return;

        vector pos = GetPosition();
        if (pos == "0 0 0".ToVector())
        {
            Print("[EoH_TownReward][SMOKE][WARN] Reward smoke marker has invalid position attempt=" + m_EoH_SmokeAttempts.ToString());
            return;
        }

        vector smokePos = pos;
        smokePos[1] = GetGame().SurfaceY(pos[0], pos[2]);

        SmokeGrenadeBase smoke = SmokeGrenadeBase.Cast(GetGame().CreateObjectEx("M18SmokeGrenade_Green", smokePos, ECE_PLACE_ON_SURFACE));
        if (!smoke)
        {
            Print("[EoH_TownReward][SMOKE][WARN] Failed to spawn M18SmokeGrenade_Green attempt=" + m_EoH_SmokeAttempts.ToString() + " pos=" + smokePos.ToString());
            return;
        }

        m_EoH_SmokeObject = smoke;

        if (smoke.GetCompEM() && smoke.GetCompEM().CanWork())
            smoke.GetCompEM().SwitchOn();

        Print("[EoH_TownReward][SMOKE] Spawned active green M18 reward smoke attempt=" + m_EoH_SmokeAttempts.ToString() + " pos=" + smokePos.ToString());
    }

    void ~EoH_RewardSmokeMarker()
    {
        if (m_EoH_SmokeObject)
            GetGame().ObjectDelete(m_EoH_SmokeObject);
    }
};
