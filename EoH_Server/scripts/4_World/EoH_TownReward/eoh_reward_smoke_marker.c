class EoH_RewardSmokeMarker extends House
{
    protected Particle m_EoH_SmokeParticle;

    void EoH_RewardSmokeMarker()
    {
        SetEventMask(EntityEvent.INIT);
    }

    override void EOnInit(IEntity other, int extra)
    {
        super.EOnInit(other, extra);

        if (!GetGame() || GetGame().IsDedicatedServer())
            return;

        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(EoH_StartSmoke, 250, false);
    }

    void EoH_StartSmoke()
    {
        if (!GetGame() || GetGame().IsDedicatedServer())
            return;

        if (m_EoH_SmokeParticle)
            return;

        m_EoH_SmokeParticle = Particle.PlayOnObject(ParticleList.EXPANSION_AIRDROP_SMOKE, this, "0 0.5 0".ToVector());
        Print("[EoH_TownReward][SMOKE] Client started reward smoke particle at " + GetPosition().ToString());
    }

    void ~EoH_RewardSmokeMarker()
    {
        if (m_EoH_SmokeParticle)
            m_EoH_SmokeParticle.Stop();
    }
};
