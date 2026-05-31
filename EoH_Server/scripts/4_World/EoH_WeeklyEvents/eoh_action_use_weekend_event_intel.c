class ActionUseWeekendEventIntelCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(2.0);
    }
}

class ActionUseWeekendEventIntel : ActionContinuousBase
{
    void ActionUseWeekendEventIntel()
    {
        m_CallbackClass = ActionUseWeekendEventIntelCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = "Decode Weekend Signal";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINonRuined();
        m_ConditionTarget = new CCTSelf();
    }

    override bool HasTarget()
    {
        return false;
    }

    override bool HasProgress()
    {
        return true;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !item)
            return false;

        return item.GetType() == "EoH_WeekendEventIntel";
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_MainItem)
            return;

        PlayerBase player = action_data.m_Player;
        ItemBase item = action_data.m_MainItem;

        if (!EoH_WeekendEventWindow.CanUseWeekendIntel(player))
            return;

        bool started = EoH_EventObjectiveManager.Get().RevealRandomObjectiveOnly();
        if (!started)
        {
            EoH_Notifications.SendToPlayer(player, "WEEKEND INTEL", "The cipher failed to resolve. Another event may already be active, or no valid weekend signals remain.");
            return;
        }

        GetGame().ObjectDelete(item);
        EoH_Notifications.SendToPlayer(player, "WEEKEND INTEL", "Encrypted weekend signal decoded. A weekend objective has been marked.");
        Print("[EoH_WeekendIntel] Generic weekend intel consumed by=" + player.GetIdentity().GetName());
    }
}

class ActionUseAltarRelayIntelCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(2.0);
    }
}

class ActionUseAltarRelayIntel : ActionContinuousBase
{
    void ActionUseAltarRelayIntel()
    {
        m_CallbackClass = ActionUseAltarRelayIntelCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = "Decode Altar Relay Intel";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINonRuined();
        m_ConditionTarget = new CCTSelf();
    }

    override bool HasTarget()
    {
        return false;
    }

    override bool HasProgress()
    {
        return true;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !item)
            return false;

        return item.GetType() == "EoH_AltarRelayIntel";
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_MainItem)
            return;

        PlayerBase player = action_data.m_Player;
        ItemBase item = action_data.m_MainItem;

        if (!EoH_WeekendEventWindow.CanUseWeekendIntel(player))
            return;

        bool started = EoH_EventObjectiveManager.Get().RevealObjectiveById("altar_relay_towers");
        if (!started)
        {
            EoH_Notifications.SendToPlayer(player, "ALTAR INTEL", "The Altar relay signal failed to resolve. Another event may already be active, or Altar has already been used.");
            return;
        }

        GetGame().ObjectDelete(item);
        EoH_Notifications.SendToPlayer(player, "ALTAR INTEL", "Altar Relay Intel decoded. The relay array has been marked.");
        Print("[EoH_WeekendIntel] Altar relay intel consumed by=" + player.GetIdentity().GetName());
    }
}
