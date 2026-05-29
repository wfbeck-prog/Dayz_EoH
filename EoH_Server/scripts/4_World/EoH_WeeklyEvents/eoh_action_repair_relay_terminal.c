class ActionRepairAltarRelayTerminalCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(10.0);
    }
}

class ActionRepairAltarRelayTerminal : ActionContinuousBase
{
    void ActionRepairAltarRelayTerminal()
    {
        m_CallbackClass = ActionRepairAltarRelayTerminalCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = "Repair Relay Terminal";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTCursor;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !target)
            return false;

        Object targetObject = target.GetObject();
        if (!targetObject)
            return false;

        if (targetObject.GetType() != "EoH_RelayRepairTerminal")
            return false;

        EoH_EventObjective active = EoH_EventObjectiveManager.Get().GetActiveObjectiveConfig();
        if (!active || active.Id != "altar_relay_towers")
            return false;

        if (EoH_EventObjectiveManager.Get().IsObjectiveCombatStarted())
            return false;

        if (!HasRequiredRepairItems(player))
            return false;

        return true;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        if (!player)
            return;

        if (!HasRequiredRepairItems(player))
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "Repair failed. Field radio and car battery are required.");
            return;
        }

        if (!ConsumeRequiredRepairItems(player))
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "Repair failed. Required repair equipment could not be consumed.");
            return;
        }

        bool activated = EoH_EventObjectiveManager.Get().ActivateObjectiveFromRepair(player);
        if (activated)
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "Field radio and car battery consumed. Relay uplink restored.");
    }

    bool HasRequiredRepairItems(PlayerBase player)
    {
        if (!player)
            return false;

        EntityAI radio = FindRadio(player);
        EntityAI battery = FindItemOnPlayer(player, "CarBattery");

        return radio && battery;
    }

    bool ConsumeRequiredRepairItems(PlayerBase player)
    {
        EntityAI radio = FindRadio(player);
        EntityAI battery = FindItemOnPlayer(player, "CarBattery");

        if (!radio || !battery)
            return false;

        GetGame().ObjectDelete(radio);
        GetGame().ObjectDelete(battery);
        return true;
    }

    EntityAI FindRadio(PlayerBase player)
    {
        EntityAI radio = FindItemOnPlayer(player, "FieldTransceiver");
        if (radio)
            return radio;

        radio = FindItemOnPlayer(player, "PersonalRadio");
        if (radio)
            return radio;

        radio = FindItemOnPlayer(player, "BaseRadio");
        if (radio)
            return radio;

        radio = FindItemOnPlayer(player, "EoH_FieldRadio");
        if (radio)
            return radio;

        return null;
    }

    EntityAI FindItemOnPlayer(PlayerBase player, string className)
    {
        if (!player || className == "")
            return null;

        array<EntityAI> items = new array<EntityAI>();
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

        foreach (EntityAI item : items)
        {
            if (!item)
                continue;

            if (item.GetType() == className || item.IsKindOf(className))
                return item;
        }

        return null;
    }
}
