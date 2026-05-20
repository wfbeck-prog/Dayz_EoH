class EoH_RewardSmokeMarker extends House
{
    protected Particle m_EoH_SmokeParticle;

    void EoH_RewardSmokeMarker()
    {
        if (!GetGame().IsDedicatedServer())
        {
            m_EoH_SmokeParticle = Particle.PlayOnObject(ParticleList.EXPANSION_AIRDROP_SMOKE, this);
        }
    }

    void ~EoH_RewardSmokeMarker()
    {
        if (m_EoH_SmokeParticle)
            m_EoH_SmokeParticle.Stop();
    }
};
