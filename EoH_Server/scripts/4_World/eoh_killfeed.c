modded class PlayerBase
{
    override void EEKilled(Object killer)
    {
        super.EEKilled(killer);

        if (!GetGame().IsServer())
            return;

        string victimName = "Unknown Survivor";
        if (GetIdentity())
            victimName = GetIdentity().GetName();

        string killerName = "Environment";
        string weaponName = "";
        float distance = 0;

        PlayerBase killerPlayer = PlayerBase.Cast(killer);
        if (killerPlayer)
        {
            if (killerPlayer.GetIdentity())
                killerName = killerPlayer.GetIdentity().GetName();

            EntityAI weapon = killerPlayer.GetHumanInventory().GetEntityInHands();
            if (weapon)
                weaponName = weapon.GetDisplayName();

            distance = vector.Distance(GetPosition(), killerPlayer.GetPosition());
        }
        else if (killer)
        {
            killerName = killer.GetType();
        }

        EoH_DiscordWebhook.SendKillFeed(victimName, killerName, weaponName, distance);
    }
}
