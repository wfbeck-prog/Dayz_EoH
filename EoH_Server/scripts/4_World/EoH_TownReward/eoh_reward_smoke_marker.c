class EoH_RewardSmokeMarker extends House
{
    protected Particle m_EoH_SmokeParticle;
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
        if (!GetGame() || GetGame().IsDedicatedServer())
            return;

        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(EoH_StartSmoke, 250, false);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(EoH_StartSmoke, 1000, false);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(EoH_StartSmoke, 2500, false);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(EoH_StartSmoke, 5000, false);
    }

    void EoH_StartSmoke()
    {
        if (!GetGame() || GetGame().IsDedicatedServer())
            return;

        m_EoH_SmokeAttempts++;

        if (m_EoH_SmokeParticle)
            return;

        vector pos = GetPosition();
        if (pos == "0 0 0".ToVector())
        {
            Print("[EoH_TownReward][SMOKE][WARN] Reward smoke marker client has invalid position on attempt=" + m_EoH_SmokeAttempts.ToString());
            return;
        }

        m_EoH_SmokeParticle = Particle.PlayOnObject(ParticleList.EXPANSION_AIRDROP_SMOKE, this, "0 1.0 0".ToVector());
        if (m_EoH_SmokeParticle)
            Print("[EoH_TownReward][SMOKE] Client started reward smoke particle attempt=" + m_EoH_SmokeAttempts.ToString() + " pos=" + pos.ToString());
        else
            Print("[EoH_TownReward][SMOKE][WARN] Particle.PlayOnObject returned null attempt=" + m_EoH_SmokeAttempts.ToString() + " pos=" + pos.ToString());
    }

    void ~EoH_RewardSmokeMarker()
    {
        if (m_EoH_SmokeParticle)
            m_EoH_SmokeParticle.Stop();
    }
};
