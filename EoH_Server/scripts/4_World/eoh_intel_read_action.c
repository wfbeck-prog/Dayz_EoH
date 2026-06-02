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
        return type == "EoH_Intel_Document" || type == "EoH_TownIntel" || type == "EoH_TraderIntel" || type == "EoH_AltarRelayIntel" || type == "EoH_WeekendEventIntel";
    }

    override void OnExecuteServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_MainItem)
            return;

        PlayerBase player = action_data.m_Player;
        ItemBase item = action_data.m_MainItem;

        string type = item.GetType();
        bool consumed = true;

        if (type == "EoH_TownIntel" || type == "EoH_Intel_Document")
        {
            EoH_IntelManager.Get().RevealTownIntel(player);
        }
        else if (type == "EoH_TraderIntel")
        {
            EoH_IntelManager.Get().RevealTraderIntel(player);
        }
        else if (type == "EoH_AltarRelayIntel")
        {
            if (!EoH_WeekendEventWindow.CanUseWeekendIntel())
            {
                EoH_Notifications.SendToPlayer(player, "WEEKEND EVENTS", EoH_WeekendEventWindow.GetClosedMessage());
                consumed = false;
            }
            else if (!EoH_EventObjectiveManager.Get().RevealObjectiveById("altar_relay_towers"))
            {
                EoH_Notifications.SendToPlayer(player, "ALTAR INTEL", "The Altar relay signal failed to resolve. Another event may already be active, or Altar has already been used.");
                consumed = false;
            }
            else
            {
                EoH_Notifications.SendToPlayer(player, "ALTAR INTEL", "Altar Relay Intel decoded. The relay array has been marked.");
            }
        }
        else if (type == "EoH_WeekendEventIntel")
        {
            if (!EoH_WeekendEventWindow.CanUseWeekendIntel())
            {
                EoH_Notifications.SendToPlayer(player, "WEEKEND EVENTS", EoH_WeekendEventWindow.GetClosedMessage());
                consumed = false;
            }
            else if (!EoH_EventObjectiveManager.Get().RevealRandomObjectiveOnly())
            {
                EoH_Notifications.SendToPlayer(player, "WEEKEND INTEL", "The cipher failed to resolve. Another event may already be active, or no valid weekend signals remain.");
                consumed = false;
            }
            else
            {
                EoH_Notifications.SendToPlayer(player, "WEEKEND INTEL", "Encrypted weekend signal decoded. A weekend objective has been marked.");
            }
        }

        if (consumed)
            item.Delete();
    }
}