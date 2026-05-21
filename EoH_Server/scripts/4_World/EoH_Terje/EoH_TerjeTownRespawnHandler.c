class EoH_TerjeTownRespawnHandler : TerjeRespawnObjectHandler
{
    override bool SetAsRespawnObject(PlayerBase player, Object object, string respawnId)
    {
        EoH_DebugOwnedRespawns(player, "SetAsRespawnObject", respawnId);

        // EoH town relay respawns are ownership-implied.
        // Players should not manually mark radio relays as respawn points.
        return false;
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
        Print("[EoH_TerjeRespawn][TOWN_HANDLER] phase=" + phase + " respawnId=" + respawnId + " player=" + player.GetIdentity().GetName() + " registryCount=" + entries.Count().ToString());
    }

    bool EoH_IsObjectValid(Object object, PlayerBase player, string respawnId)
    {
        if (!object || !player)
            return false;

        EoH_CaptureRelay_Base relay = EoH_CaptureRelay_Base.Cast(object);
        if (!relay)
            return false;

        string town = relay.GetEoHTownName();
        if (town == "")
            return false;

        string playerGroupID = EoH_GroupHelper.GetGroupID(player);
        if (playerGroupID == "")
            return false;

        EoH_WorldStateTownState state = EoH_WorldStateManager.Get().GetTownState(town);
        if (!state || state.OwnerGroupID == "")
            return false;

        return state.OwnerGroupID == playerGroupID;
    }
}
