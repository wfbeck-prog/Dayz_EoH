class EoH_ActionReadIntel : ActionSingleUseBase
{
    void EoH_ActionReadIntel()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_VIEWCOMPASS;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = "Read Intel";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTNone;
    }

    override bool HasTarget()
    {
        return false;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !item)
            return false;

        if (item.IsRuined())
            return false;

        string type = item.GetType();
        return type == "EoH_Intel_Document" || type == "EoH_TownIntel" || type == "EoH_TraderIntel";
    }

    override void OnExecuteServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_MainItem)
            return;

        PlayerBase player = action_data.m_Player;
        ItemBase item = action_data.m_MainItem;

        string type = item.GetType();

        if (type == "EoH_TownIntel" || type == "EoH_Intel_Document")
        {
            EoH_IntelManager.Get().RevealTownIntel(player);
        }
        else if (type == "EoH_TraderIntel")
        {
            EoH_IntelManager.Get().RevealTraderIntel(player);
        }

        item.Delete();
    }
}
