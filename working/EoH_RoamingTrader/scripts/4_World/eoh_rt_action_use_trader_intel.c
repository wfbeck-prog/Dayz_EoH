// Action: Use Trader Intel
// Consumes intel item and triggers global trader reveal via EoH_RT_IntelBridge

class EoH_ActionUseTraderIntel : ActionSingleUseBase
{
    void EoH_ActionUseTraderIntel()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
        m_Text = "Use Trader Intel";
    }

    override bool HasTarget()
    {
        return false;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        return item != NULL;
    }

    override void OnExecuteServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_MainItem)
            return;

        PlayerBase player = action_data.m_Player;
        ItemBase item = action_data.m_MainItem;

        string className = item.GetType();

        // Trigger intel bridge (does NOT replace discovery marker path)
        EoH_RT_IntelBridge.RevealTraderFromIntel(player, className);

        // Consume the intel item
        item.Delete();
    }
}
