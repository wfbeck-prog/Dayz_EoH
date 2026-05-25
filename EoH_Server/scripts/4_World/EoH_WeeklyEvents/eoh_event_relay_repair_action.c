class EoH_ActionRepairEventRelayCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(12.0);
    }
}

class EoH_ActionRepairEventRelay : ActionContinuousBase
{
    static const float REPAIR_RADIUS = 35.0;
    static const string REQUIRED_TRANSCEIVER = "BaseRadio";
    static const string REQUIRED_BATTERY = "CarBattery";

    void EoH_ActionRepairEventRelay()
    {
        m_CallbackClass = EoH_ActionRepairEventRelayCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
    }

    override string GetText()
    {
        return "Repair Relay Tower";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !player.IsAlive())
            return false;

        EoH_EventObjectiveManager mgr = EoH_EventObjectiveManager.Get();
        if (!mgr || !mgr.HasActiveObjective())
            return false;

        if (mgr.IsObjectiveCombatStarted())
            return false;

        if (!mgr.IsPlayerNearActiveObjective(player, REPAIR_RADIUS))
            return false;

        return HasRequiredRepairParts(player);
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player)
            return;

        PlayerBase player = action_data.m_Player;

        if (!HasRequiredRepairParts(player))
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "Repair failed. A Field Transceiver/BaseRadio and Car Battery are required.");
            return;
        }

        EoH_EventObjectiveManager mgr = EoH_EventObjectiveManager.Get();
        if (!mgr || !mgr.IsPlayerNearActiveObjective(player, REPAIR_RADIUS))
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "You are too far from the relay array.");
            return;
        }

        ConsumeRequiredRepairParts(player);

        if (mgr.ActivateObjectiveFromRepair(player))
        {
            EoH_Notifications.SendToPlayer(player, "RELAY REPAIR", "Relay repair complete. The signal is live.");
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

modded class PlayerBase
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionRepairEventRelay);
    }
}
