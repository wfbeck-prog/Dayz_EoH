class EoH_TerjeTerritoryRespawnHandler : TerjeRespawnObjectHandler
{
    override bool SetAsRespawnObject(PlayerBase player, Object object, string respawnId)
    {
        EoH_DebugOwnedRespawns(player, "SetAsRespawnObject", respawnId);
        return EoH_IsObjectValid(object, player, respawnId);
    }

    override bool RespawnOnObject(PlayerBase player, Object object, string respawnId)
    {
        EoH_DebugOwnedRespawns(player, "RespawnOnObject", respawnId);
        return EoH_IsObjectValid(object, player, respawnId);
    }

    void EoH_DebugOwnedRespawns(PlayerBase player, string phase, string respawnId)
    {
        if (!player || !player.GetIdentity())
            return;

        ref array<ref EoH_TerjeOwnedRespawnEntry> entries = EoH_TerjeOwnedRespawnRegistry.BuildOwnedRespawns(player);
        Print("[EoH_TerjeRespawn][TERRITORY_HANDLER] phase=" + phase + " respawnId=" + respawnId + " player=" + player.GetIdentity().GetName() + " registryCount=" + entries.Count().ToString());
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
