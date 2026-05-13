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

        // If DayZ reports the victim as their own killer, treat it as environment/bleed-out.
        if (killer == this)
        {
            killerName = "Environment";
            weaponName = "Bleed-out / Exposure";
        }
        else if (killerPlayer)
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
            string killerType = killer.GetType();

            if (killerType.Contains("eAI") || killerType.Contains("Infected") || killerType.Contains("Zmb") || killerType.Contains("Animal") || killerType.Contains("Wolf") || killerType.Contains("Bear"))
                killerName = "AI Survivor";
            else
                killerName = killerType;
        }

        EoH_DiscordWebhook.SendKillFeed(victimName, killerName, weaponName, distance);

        string redLedgerReason = "";

        if (killerIsHuman && distance >= 500)
            redLedgerReason = "Extreme Longshot";
        else if (killerIsHuman && distance >= 300)
            redLedgerReason = "Long Range Kill";

        vector pos = GetPosition();

        if (killerIsHuman && vector.Distance(pos, "4700 0 10200") < 1200)
        {
            if (redLedgerReason != "")
                redLedgerReason += ", ";

            redLedgerReason += "Military Zone Engagement";
        }

        if (redLedgerReason != "")
            EoH_DiscordWebhook.SendHighValueKill(victimName, killerName, weaponName, distance, redLedgerReason);
    }
}
