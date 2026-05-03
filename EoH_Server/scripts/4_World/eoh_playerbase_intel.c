modded class PlayerBase
{
    override void EEKilled(Object killer)
    {
        super.EEKilled(killer);

        if (!GetIdentity())
            return;

        EoH_IntelManager.Get().ResetPlayerIntelUsage(GetIdentity().GetId());
    }
};
