class EoH_TerjeTerritoryRespawnHandler : TerjeRespawnObjectHandler
{
    override bool IsObjectValid(Object object, PlayerBase player, string respawnId, TerjeXmlObject xmlObject)
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

    override bool GetPlayerSpawnPos(Object object, PlayerBase player, string respawnId, TerjeXmlObject xmlObject, out vector playerPos, out vector playerOri)
    {
        if (!IsObjectValid(object, player, respawnId, xmlObject))
            return false;

        vector basePos = object.GetPosition();
        playerPos = EoH_TerritorySpawnBridge.SafeSpawnPosition(basePos, 4.0);
        playerOri = "0 0 0".ToVector();
        return true;
    }

    override string GetFailMessage(Object object, PlayerBase player, string respawnId, TerjeXmlObject xmlObject)
    {
        return "Your group no longer controls this territory.";
    }
}
