class ActionRepairAltarRelayTerminalCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(10.0);
    }
}

class ActionRepairAltarRelayTerminal : ActionContinuousBase
{
    protected int m_LastDebugLogTime;

    void ActionRepairAltarRelayTerminal()
    {
        m_CallbackClass = ActionRepairAltarRelayTerminalCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = "Repair Relay Terminal";
        m_LastDebugLogTime = 0;
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
        {
            DebugBlocked("no_target_object", player, null, null, false, false);
            return false;
        }

        Object parentObject = null;
        EntityAI targetEntity = EntityAI.Cast(targetObject);
        if (targetEntity)
            parentObject = targetEntity.GetHierarchyParent();

        bool validTarget = IsRelayTerminalTarget(targetObject);
        if (!validTarget && parentObject)
            validTarget = IsRelayTerminalTarget(parentObject);

        EoH_EventObjective active = EoH_EventObjectiveManager.Get().GetActiveObjectiveConfig();
        bool altarActive = active && active.Id == "altar_relay_towers";
        bool combatStarted = EoH_EventObjectiveManager.Get().IsObjectiveCombatStarted();
        bool hasRadio = FindRadio(player) != null;
        bool hasBattery = FindItemOnPlayer(player, "CarBattery") != null;

        if (!validTarget)
        {
            DebugBlocked("bad_target", player, targetObject, parentObject, hasRadio, hasBattery);
            return false;
        }

        if (!altarActive)
        {
            DebugBlocked("altar_not_active", player, targetObject, parentObject, hasRadio, hasBattery);
            return false;
        }

        if (combatStarted)
        {
            DebugBlocked("already_repaired", player, targetObject, parentObject, hasRadio, hasBattery);
            return false;
        }

        if (!hasRadio || !hasBattery)
        {
            DebugBlocked("missing_items", player, targetObject, parentObject, hasRadio, hasBattery);
            return false;
        }

        DebugAllowed(player, targetObject, parentObject, hasRadio, hasBattery);
        return true;
    }

    bool IsRelayTerminalTarget(Object obj)
    {
        if (!obj)
            return false;

        string type = obj.GetType();
        if (type == "EoH_RelayRepairActionAnchor")
            return true;

        if (obj.IsKindOf("EoH_RelayRepairActionAnchor"))
            return true;

        if (type == "EoH_RelayRepairTerminal")
            return true;

        if (obj.IsKindOf("EoH_RelayRepairTerminal"))
            return true;

        if (type.Contains("RelayRepairActionAnchor"))
            return true;

        if (type.Contains("RelayRepairTerminal"))
            return true;

        if (type.Contains("RepairTerminal"))
            return true;

        return false;
    }

    void DebugBlocked(string reason, PlayerBase player, Object targetObject, Object parentObject, bool hasRadio, bool hasBattery)
    {
        if (!ShouldDebug())
            return;

        string targetType = "null";
        string parentType = "null";
        string activeId = "none";

        if (targetObject)
            targetType = targetObject.GetType();
        if (parentObject)
            parentType = parentObject.GetType();

        EoH_EventObjective active = EoH_EventObjectiveManager.Get().GetActiveObjectiveConfig();
        if (active)
            activeId = active.Id;

        Print("[EoH_RelayAction][DEBUG] blocked=" + reason + " targetType=" + targetType + " parentType=" + parentType + " activeObjective=" + activeId + " hasRadio=" + hasRadio.ToString() + " hasBattery=" + hasBattery.ToString());
    }

    void DebugAllowed(PlayerBase player, Object targetObject, Object parentObject, bool hasRadio, bool hasBattery)
    {
        if (!ShouldDebug())
            return;

        string targetType = "null";
        string parentType = "null";
        if (targetObject)
            targetType = targetObject.GetType();
        if (parentObject)
            parentType = parentObject.GetType();

        Print("[EoH_RelayAction][DEBUG] allowed targetType=" + targetType + " parentType=" + parentType + " hasRadio=" + hasRadio.ToString() + " hasBattery=" + hasBattery.ToString());
    }

    bool ShouldDebug()
    {
        int now = GetGame().GetTime();
        if (m_LastDebugLogTime > 0 && now - m_LastDebugLogTime < 3000)
            return false;

        m_LastDebugLogTime = now;
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
