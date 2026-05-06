class EoH_TerjeTerritoryRespawnHandler : TerjeRespawnObjectHandler
{
    override bool SetAsRespawnObject(PlayerBase player, Object object, string respawnId)
    {
        return EoH_IsObjectValid(object, player, respawnId);
    }

    override bool RespawnOnObject(PlayerBase player, Object object, string respawnId)
    {
        return EoH_IsObjectValid(object, player, respawnId);
    }

    bool EoH_IsObjectValid(Object object, PlayerBase player, string respawnId)
    {
        if (!object || !player)
            return false;

        string ownerGroupID = EoH_TerritoryOwnershipRegistry.GetOwnerForObject(object);
        if (ownerGroupID == "")
            return false;

        string playerGroupID = EoH_GroupHelper.GetGroupID(player);
        if (playerGroupID == "")
            return false;

        return ownerGroupID == playerGroupID;
    }
}
