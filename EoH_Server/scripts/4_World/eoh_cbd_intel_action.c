class EoH_ActionUseCBDIntel : ActionSingleUseBase
{
    void EoH_ActionUseCBDIntel()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
    }

    override string GetText()
    {
        return "Decode CBD Intel";
    }

    override bool HasTarget()
    {
        return false;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !item)
            return false;

        return item.GetType() == "EoH_CBDIntel";
    }

    override void OnExecuteServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_MainItem)
            return;

        EoH_IntelManager.Get().RevealCBDIntel(action_data.m_Player);
        action_data.m_MainItem.Delete();
    }
};

modded class PlayerBase
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionUseCBDIntel);
    }
};
