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

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !item)
            return false;

        return EoH_IsIntelItem(item.GetType());
    }

    override void OnExecuteServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        ItemBase intel = action_data.m_MainItem;

        if (!player || !intel)
            return;

        string type = intel.GetType();

        if (type == "EoH_TraderIntel")
        {
            EoH_IntelManager.Get().RevealTraderIntel(player);
        }
        else
        {
            EoH_IntelManager.Get().RevealTownIntel(player);
        }

        intel.Delete();
    }
};

bool EoH_IsIntelItem(string type)
{
    return type == "EoH_Intel_Document" || type == "EoH_TownIntel" || type == "EoH_TraderIntel";
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
