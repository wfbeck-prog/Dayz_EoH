class EoH_ActionReadIntel : ActionInteractBase
{
    void EoH_ActionReadIntel()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = "Read Intel";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTSelf;
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

    override void OnStartServer(ActionData action_data)
    {
        super.OnStartServer(action_data);

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
