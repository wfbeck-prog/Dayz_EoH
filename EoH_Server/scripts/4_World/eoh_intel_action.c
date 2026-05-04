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
        return item && item.GetType() == "EoH_Intel_Document";
    }

    override void OnExecuteServer(ActionData action_data)
{
    PlayerBase player = action_data.m_Player;

    EoH_IntelManager.Get().RevealTownIntel(player);

    action_data.m_MainItem.Delete();
}
};
