modded class TerritoryFlagKit extends KitBase
{
    override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
    {
        if (!GetGame().IsServer())
            return;

        PlayerBase playerBase = PlayerBase.Cast(player);
        if (!playerBase)
            return;

        string groupID = EoH_GroupHelper.GetGroupID(playerBase);
        if (groupID == "")
        {
            playerBase.MessageStatus("You must be in a group to place a territory flag.");
            return;
        }

        TerritoryFlag placedFlag = TerritoryFlag.Cast(
            GetGame().CreateObjectEx("TerritoryFlag", position, ECE_PLACE_ON_SURFACE)
        );

        if (!placedFlag)
            return;

        placedFlag.SetPosition(position);
        placedFlag.SetOrientation(orientation);

        HideAllSelections();
        SetIsDeploySound(true);

        placedFlag.GetConstruction().BuildPartServer(playerBase, "base", AT_BUILD_PART);
        placedFlag.GetConstruction().BuildPartServer(playerBase, "support", AT_BUILD_PART);
        placedFlag.GetConstruction().BuildPartServer(playerBase, "pole", AT_BUILD_PART);

        int slotId = InventorySlots.GetSlotIdFromString("material_fpole_flag");

EntityAI attachment = placedFlag.GetInventory().FindAttachment(slotId);

if (!attachment)
{
    placedFlag.GetInventory().CreateAttachment("Flag_DayZ");
}

        placedFlag.GetConstruction().BuildPartServer(playerBase, "flag", AT_BUILD_PART);

        // 🔥 Ownership assignment (critical)
        placedFlag.SetEoHOwner(groupID);

        playerBase.MessageStatus("Territory claimed for your group.");
    }
}
