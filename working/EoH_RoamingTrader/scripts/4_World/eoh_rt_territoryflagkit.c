// EoH_RoamingTrader - Territory Flag Auto-Build Patch
// Drop this file into: EoH_RoamingTrader/scripts/4_World/

modded class TerritoryFlagKit extends KitBase
{
    override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
    {
        if (!GetGame().IsServer())
            return;

        PlayerBase playerBase = PlayerBase.Cast(player);
        if (!playerBase)
            return;

        TerritoryFlag placedFlag = TerritoryFlag.Cast(GetGame().CreateObjectEx("TerritoryFlag", position, ECE_PLACE_ON_SURFACE));
        if (!placedFlag)
            return;

        placedFlag.SetPosition(position);
        placedFlag.SetOrientation(orientation);

        HideAllSelections();
        SetIsDeploySound(true);

        placedFlag.GetConstruction().BuildPartServer(playerBase, "base", AT_BUILD_PART);
        placedFlag.GetConstruction().BuildPartServer(playerBase, "support", AT_BUILD_PART);
        placedFlag.GetConstruction().BuildPartServer(playerBase, "pole", AT_BUILD_PART);

        if (!placedFlag.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("material_fpole_flag")))
        {
            placedFlag.GetInventory().CreateAttachment("Flag_DayZ");
        }

        placedFlag.GetConstruction().BuildPartServer(playerBase, "flag", AT_BUILD_PART);
    }
}
