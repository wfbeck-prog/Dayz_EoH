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

        Print("[EoH_TownReward][SMOKE] Client init reward smoke marker pos=" + GetPosition().ToString());

        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(EoH_StartSmoke, 100, false);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(EoH_StartSmoke, 500, false);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(EoH_StartSmoke, 1500, false);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(EoH_StartSmoke, 3000, false);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(EoH_StartSmoke, 6000, false);
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
            Print("[EoH_TownReward][SMOKE][WARN] Reward smoke marker client has invalid position attempt=" + m_EoH_SmokeAttempts.ToString());
            return;
        }

        vector smokePos = pos;
        smokePos[1] = smokePos[1] + 1.0;

        m_EoH_SmokeParticle = Particle.PlayInWorld(ParticleList.EXPANSION_AIRDROP_SMOKE, smokePos);
        if (m_EoH_SmokeParticle)
        {
            Print("[EoH_TownReward][SMOKE] Client started reward smoke world particle attempt=" + m_EoH_SmokeAttempts.ToString() + " pos=" + smokePos.ToString());
            return;
        }

        Print("[EoH_TownReward][SMOKE][WARN] Expansion airdrop smoke particle returned null attempt=" + m_EoH_SmokeAttempts.ToString() + " pos=" + smokePos.ToString());
    }

    void ~EoH_RewardSmokeMarker()
    {
        if (m_EoH_SmokeParticle)
            m_EoH_SmokeParticle.Stop();
    }
};
