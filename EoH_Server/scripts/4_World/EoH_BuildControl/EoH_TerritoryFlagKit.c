modded class TerritoryFlagKit extends KitBase
{
    override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
    {
        super.OnPlacementComplete(player, position, orientation);

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

        Object flagObject = GetGame().CreateObjectEx("TerritoryFlag", position, ECE_PLACE_ON_SURFACE);
        TerritoryFlag placedFlag = TerritoryFlag.Cast(flagObject);
        Flag_Base placedFlagBase = Flag_Base.Cast(flagObject);

        if (!placedFlag || !placedFlagBase)
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

        placedFlagBase.SetEoHOwner(groupID);

        playerBase.MessageStatus("Territory claimed for your group.");
    }
}
