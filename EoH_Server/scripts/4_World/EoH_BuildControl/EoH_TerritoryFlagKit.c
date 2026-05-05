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

        Object placed = GetGame().GetObjectByNetworkId(GetNetworkIDLow(), GetNetworkIDHigh());
        TerritoryFlag flag = TerritoryFlag.Cast(placed);
        Flag_Base flagBase = Flag_Base.Cast(placed);

        if (!flag || !flagBase)
            return;

        flag.GetConstruction().BuildPartServer(playerBase, "base", AT_BUILD_PART);
        flag.GetConstruction().BuildPartServer(playerBase, "support", AT_BUILD_PART);
        flag.GetConstruction().BuildPartServer(playerBase, "pole", AT_BUILD_PART);

        int slotId = InventorySlots.GetSlotIdFromString("material_fpole_flag");
        EntityAI attachment = flag.GetInventory().FindAttachment(slotId);

        if (!attachment)
        {
            flag.GetInventory().CreateAttachment("Flag_DayZ");
        }

        flag.GetConstruction().BuildPartServer(playerBase, "flag", AT_BUILD_PART);

        flagBase.SetEoHOwner(groupID);

        playerBase.MessageStatus("Territory claimed.");
    }
}
