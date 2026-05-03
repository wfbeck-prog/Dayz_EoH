modded class PlayerBase
{
    void OnKilled(Object killer)
    {
        super.OnKilled(killer);

        if (!GetIdentity())
            return;

        EoH_IntelManager.Get().ResetPlayerIntelUsage(GetIdentity().GetId());
    }
};
