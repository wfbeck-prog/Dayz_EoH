class EoH_TerjeTownRespawnHandler : TerjeRespawnObjectHandler
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
