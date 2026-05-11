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

        // Do not send AI vs AI deaths to Discord.
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

        string redLedgerReason = "";

        if (distance >= 500)
            redLedgerReason = "Extreme Longshot";
        else if (distance >= 300)
            redLedgerReason = "Long Range Kill";

        vector pos = GetPosition();

        if (vector.Distance(pos, "4700 0 10200") < 1200)
        {
            if (redLedgerReason != "")
                redLedgerReason += ", ";

            redLedgerReason += "Military Zone Engagement";
        }

        if (redLedgerReason != "")
            EoH_DiscordWebhook.SendHighValueKill(victimName, killerName, weaponName, distance, redLedgerReason);
    }
}
