class EoH_ActionUseLootRoomIntel : ActionInteractBase
{
    void EoH_ActionUseLootRoomIntel()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
    }

    override string GetText()
    {
        return "Decode Loot Room Intel";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !item)
            return false;

        return item.GetType() == "EoH_CBDIntel" || item.GetType() == "EoH_LootRoomIntel";
    }

    override void OnStartServer(ActionData action_data)
    {
        HandleLootRoomIntel(action_data);
    }

    override void OnExecuteServer(ActionData action_data)
    {
        HandleLootRoomIntel(action_data);
    }

    void HandleLootRoomIntel(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_MainItem)
            return;

        string itemType = action_data.m_MainItem.GetType();
        if (itemType != "EoH_CBDIntel" && itemType != "EoH_LootRoomIntel")
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
        AddAction(EoH_ActionUseLootRoomIntel);
    }
};
