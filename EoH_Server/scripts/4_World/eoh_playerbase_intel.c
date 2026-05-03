modded class PlayerBase
{
    override void OnPlayerKilled(Object killer)
    {
        super.OnPlayerKilled(killer);

        if (!GetIdentity())
            return;

        EoH_IntelManager.Get().ResetPlayerIntelUsage(GetIdentity().GetId());
    }
};
