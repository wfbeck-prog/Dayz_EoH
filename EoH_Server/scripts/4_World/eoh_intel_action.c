class ActionUseIntel: ActionSingleUseBase
{
    void ActionUseIntel()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
    }

    override string GetText()
    {
        return "Use Intel";
    }

    override bool HasTarget()
    {
        return false;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !item)
            return false;

        return EoH_IsIntelItem(item.GetType());
    }

    override void OnExecuteServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_MainItem)
            return;

        PlayerBase player = action_data.m_Player;
        ItemBase intel = action_data.m_MainItem;

        string type = intel.GetType();

        if (type == "EoH_TraderIntel")
        {
            EoH_IntelManager.Get().RevealTraderIntel(player);
            Print("[EoH_Intel] Manual trader intel used player=" + player.GetIdentity().GetName());
        }
        else if (type == "EoH_CBDIntel" || type == "EoH_LootRoomIntel")
        {
            EoH_IntelManager.Get().RevealCBDIntel(player);
            Print("[EoH_Intel] Manual loot room intel used player=" + player.GetIdentity().GetName());
        }
        else if (type == "EoH_TownIntel" || type == "EoH_Intel_Document")
        {
            EoH_IntelManager.Get().RevealTownIntel(player);
            Print("[EoH_Intel] Manual town intel used player=" + player.GetIdentity().GetName());
        }

        intel.Delete();
    }
};

bool EoH_IsIntelItem(string type)
{
    return type == "EoH_Intel_Document" || type == "EoH_TownIntel" || type == "EoH_TraderIntel" || type == "EoH_CBDIntel" || type == "EoH_LootRoomIntel";
}

modded class ItemBase
{
    override void SetActions()
    {
        super.SetActions();

        if (EoH_IsIntelItem(GetType()))
            AddAction(ActionUseIntel);
    }
};
