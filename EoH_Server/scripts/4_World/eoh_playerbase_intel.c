modded class PlayerBase
{
    override void EEOnKilled(EntityAI killer)
    {
        super.EEOnKilled(killer);

        if (!GetIdentity())
            return;

        EoH_IntelManager.Get().ResetPlayerIntelUsage(GetIdentity().GetId());
    }
};
