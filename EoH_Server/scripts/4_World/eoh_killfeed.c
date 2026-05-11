modded class PlayerBase
{
    override void EEKilled(Object killer)
    {
        super.EEKilled(killer);

        if (!GetGame().IsServer())
            return;

        bool victimIsHuman = GetIdentity() != null;

        PlayerBase killerPlayer = PlayerBase.Cast(killer);
        bool killerIsHuman = false;
        if (killerPlayer && killerPlayer.GetIdentity())
            killerIsHuman = true;

        // Do not send AI vs AI deaths to Discord. Keep player deaths and player kills visible.
        if (!victimIsHuman && !killerIsHuman)
            return;

        string victimName = "AI Survivor";
        if (GetIdentity())
            victimName = GetIdentity().GetName();

        string killerName = "Environment";
        string weaponName = "";
        float distance = 0;

        if (killerPlayer)
        {
            if (killerPlayer.GetIdentity())
                killerName = killerPlayer.GetIdentity().GetName();
            else
                killerName = "AI Survivor";

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
