modded class PlayerBase
{
    override void OnSelectPlayer()
    {
        super.OnSelectPlayer();
        if (GetGame().IsServer())
            EoH_SCB_Core.ScheduleEnforce(this);
    }
}

class EoH_SCB_Core
{
    protected static ref EoH_SCB_Config m_Config;

    static ref EoH_SCB_Config GetConfig()
    {
        if (!m_Config)
            m_Config = new EoH_SCB_Config();
        return m_Config;
    }

    static void ScheduleEnforce(PlayerBase player)
    {
        if (!player) return;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EnforcePlayer, GetConfig().RestoreDelayMs, false, player);
    }

    static void EnforcePlayer(PlayerBase player)
    {
        if (!player || !player.IsAlive()) return;

        string allowed = GetAllowed(player);
        A6_SecureContainer_Base current = player.GetSecureContainer();

        if (current && current.GetType() != allowed)
        {
            player.GetInventory().LocalDestroyEntity(current);
            current = null;
        }

        if (!current && allowed != string.Empty)
        {
            player.GetInventory().CreateInInventory(allowed);
        }

        Cleanup(player);
    }

    static string GetAllowed(PlayerBase player)
    {
        int lvl = GetLevel(player);
        EoH_SCB_Config cfg = GetConfig();

        if (lvl >= cfg.Level4x4) return cfg.Tier4x4;
        if (lvl >= cfg.Level3x3) return cfg.Tier3x3;
        if (lvl >= cfg.Level2x2) return cfg.Tier2x2;
        return string.Empty;
    }

    static int GetLevel(PlayerBase player)
    {
        if (!player || !player.GetTerjeSkills()) return 0;
        return player.GetTerjeSkills().GetSkillLevel("surv");
    }

    static void Cleanup(PlayerBase player)
    {
        array<EntityAI> items = new array<EntityAI>;
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

        int count = 0;
        foreach (EntityAI item : items)
        {
            if (item && item.IsKindOf("A6_SecureContainer_Base"))
            {
                count++;
                if (count > 1)
                    player.GetInventory().LocalDestroyEntity(item);
            }
        }
    }
}
