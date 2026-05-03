modded class PlayerBase
{
    override void EEOnDeath(EntityAI killer)
    {
        super.EEOnDeath(killer);

        if (!GetIdentity())
            return;

        EoH_IntelManager.Get().ResetPlayerIntelUsage(GetIdentity().GetId());
    }
};
