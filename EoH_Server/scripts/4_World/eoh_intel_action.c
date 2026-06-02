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

        bool isSpecificEventIntel = (type == "EoH_EventIntel" || type == "EoH_AltarRelayIntel");
        bool isGenericWeekendEventIntel = (type == "EoH_WeekendEventIntel");
        bool isEventIntel = isSpecificEventIntel || isGenericWeekendEventIntel;
        EoH_EventObjectiveManager eventMgr = EoH_EventObjectiveManager.Get();

        if (!isEventIntel && eventMgr && !eventMgr.IsIntelAvailable())
        {
            EoH_Notifications.SendToPlayer(player, "INTEL CHANNELS DOWN", "The relay network is dark. Restore the tower terminal before decoding more field intel.");
            Print("[EoH_Intel][BLOCKED] Non-event intel blocked by active weekly event type=" + type + " player=" + player.GetIdentity().GetName());
            return;
        }

        if (type == "EoH_TraderIntel")
        {
            EoH_IntelManager.Get().RevealTraderIntel(player);
            Print("[EoH_Intel] Manual trader intel used player=" + player.GetIdentity().GetName());
        }
        else if (type == "EoH_WeekendEventIntel")
        {
            if (!EoH_WeekendEventWindow.CanUseWeekendIntel())
            {
                EoH_Notifications.SendToPlayer(player, "WEEKEND EVENTS", EoH_WeekendEventWindow.GetClosedMessage());
                return;
            }

            if (!eventMgr.RevealRandomObjectiveOnly())
            {
                EoH_Notifications.SendToPlayer(player, "WEEKEND INTEL", "The cipher failed to resolve. Another event may already be active, or no valid weekend signals remain.");
                return;
            }

            EoH_Notifications.SendToPlayer(player, "WEEKEND INTEL", "Encrypted weekend signal decoded. A weekend objective has been marked.");
            Print("[EoH_Intel] Manual generic weekend event intel used player=" + player.GetIdentity().GetName());
        }
        else if (isSpecificEventIntel)
        {
            EoH_IntelManager.Get().RevealWeeklyEventIntel(player);
            Print("[EoH_Intel] Manual weekly event intel used player=" + player.GetIdentity().GetName());
        }
        else if (type == "EoH_CBDIntel" || type == "EoH_LootRoomIntel")
        {
            EoH_IntelManager.Get().RevealCBDIntel(player);
            Print("[EoH_Intel] Manual loot room intel used player=" + player.GetIdentity().GetName());
        }
        else if (type == "EoH_TownIntel" || type == "EoH_Intel_Document")
        {
            EoH_IntelManager.BeginManualTownIntel();
            EoH_IntelManager.Get().RevealTownIntel(player);
            EoH_IntelManager.EndManualTownIntel();

            Print("[EoH_Intel] Manual town intel used player=" + player.GetIdentity().GetName());
        }

        intel.Delete();
    }
};

bool EoH_IsIntelItem(string type)
{
    return type == "EoH_Intel_Document" || type == "EoH_TownIntel" || type == "EoH_TraderIntel" || type == "EoH_CBDIntel" || type == "EoH_LootRoomIntel" || type == "EoH_EventIntel" || type == "EoH_AltarRelayIntel" || type == "EoH_WeekendEventIntel";
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