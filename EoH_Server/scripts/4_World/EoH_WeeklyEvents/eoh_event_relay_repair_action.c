class EoH_RelayRepairTerminal : Radio
{
    override bool CanPutInCargo(EntityAI parent)
    {
        return false;
    }

    override bool CanPutIntoHands(EntityAI parent)
    {
        return false;
    }

    override bool CanRemoveFromCargo(EntityAI parent)
    {
        return false;
    }
}

bool EoH_IsRelayRepairObject(string type)
{
    return type == "EoH_RelayRepairNode" || type == "EoH_RelayRepairTerminal" || type == "EoH_RelayRepairActionAnchor";
}

class EoH_ActionRepairEventRelay : ActionInteractBase
{
    static const float REPAIR_RADIUS = 35.0;
    static const string REQUIRED_TRANSCEIVER = "BaseRadio";
    static const string REQUIRED_BATTERY = "CarBattery";

    void EoH_ActionRepairEventRelay()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
    }

    override string GetText()
    {
        return "Repair Tower Terminal";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !player.IsAlive())
            return false;

        if (!target || !target.GetObject())
            return false;

        Object targetObj = target.GetObject();
        if (!targetObj)
            return false;

        string type = targetObj.GetType();
        if (!EoH_IsRelayRepairObject(type))
            return false;

        EoH_EventObjectiveManager mgr = EoH_EventObjectiveManager.Get();
        if (!mgr || !mgr.HasActiveObjective())
            return false;

        if (mgr.IsObjectiveCombatStarted())
            return false;

        if (!mgr.IsPlayerNearActiveObjective(player, REPAIR_RADIUS))
            return false;

        return true;
    }

    override void OnStartServer(ActionData action_data)
    {
        EoH_HandleRelayRepair(action_data, "OnStartServer");
    }

    override void OnExecuteServer(ActionData action_data)
    {
        EoH_HandleRelayRepair(action_data, "OnExecuteServer");
    }

    void EoH_HandleRelayRepair(ActionData action_data, string source)
    {
        if (!action_data || !action_data.m_Player)
            return;

        PlayerBase player = action_data.m_Player;

        if (!action_data.m_Target || !action_data.m_Target.GetObject())
        {
            Print("[EoH_EventRepair][DEBUG] " + source + " failed no target player=" + player.GetIdentity().GetName());
            return;
        }

        Object targetObj = action_data.m_Target.GetObject();
        string targetType = targetObj.GetType();
        Print("[EoH_EventRepair][DEBUG] " + source + " clicked by " + player.GetIdentity().GetName() + " targetType=" + targetType + " pos=" + targetObj.GetPosition().ToString());

        if (!EoH_IsRelayRepairObject(targetType))
            return;

        EoH_EventObjectiveManager mgr = EoH_EventObjectiveManager.Get();
        if (!mgr || !mgr.HasActiveObjective())
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "No active tower signal is available for repair.");
            return;
        }

        if (mgr.IsObjectiveCombatStarted())
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "The tower terminal has already been restored.");
            return;
        }

        if (!mgr.IsPlayerNearActiveObjective(player, REPAIR_RADIUS))
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "You are too far from the tower terminal.");
            return;
        }

        if (!HasRequiredRepairParts(player))
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "Repair failed. A Field Transceiver and car battery are required.");
            return;
        }

        ConsumeRequiredRepairParts(player);

        if (mgr.ActivateObjectiveFromRepair(player))
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "Tower terminal repaired. The relay network is live.");
        }
    }

    bool HasRequiredRepairParts(PlayerBase player)
    {
        if (!player)
            return false;

        return FindItemInEntity(player, REQUIRED_TRANSCEIVER) && FindItemInEntity(player, REQUIRED_BATTERY);
    }

    void ConsumeRequiredRepairParts(PlayerBase player)
    {
        DeleteFirstItemInEntity(player, REQUIRED_TRANSCEIVER);
        DeleteFirstItemInEntity(player, REQUIRED_BATTERY);
    }

    bool FindItemInEntity(EntityAI root, string typeName)
    {
        if (!root || typeName == "")
            return false;

        if (root.GetType() == typeName)
            return true;

        if (!root.GetInventory())
            return false;

        CargoBase cargo = root.GetInventory().GetCargo();
        if (cargo)
        {
            for (int i = 0; i < cargo.GetItemCount(); i++)
            {
                EntityAI cargoItem = EntityAI.Cast(cargo.GetItem(i));
                if (FindItemInEntity(cargoItem, typeName))
                    return true;
            }
        }

        int attachmentCount = root.GetInventory().AttachmentCount();
        for (int a = 0; a < attachmentCount; a++)
        {
            EntityAI attachment = root.GetInventory().GetAttachmentFromIndex(a);
            if (FindItemInEntity(attachment, typeName))
                return true;
        }

        return false;
    }

    bool DeleteFirstItemInEntity(EntityAI root, string typeName)
    {
        if (!root || typeName == "")
            return false;

        if (root.GetType() == typeName)
        {
            GetGame().ObjectDelete(root);
            return true;
        }

        if (!root.GetInventory())
            return false;

        CargoBase cargo = root.GetInventory().GetCargo();
        if (cargo)
        {
            for (int i = 0; i < cargo.GetItemCount(); i++)
            {
                EntityAI cargoItem = EntityAI.Cast(cargo.GetItem(i));
                if (DeleteFirstItemInEntity(cargoItem, typeName))
                    return true;
            }
        }

        int attachmentCount = root.GetInventory().AttachmentCount();
        for (int a = 0; a < attachmentCount; a++)
        {
            EntityAI attachment = root.GetInventory().GetAttachmentFromIndex(a);
            if (DeleteFirstItemInEntity(attachment, typeName))
                return true;
        }

        return false;
    }
}
