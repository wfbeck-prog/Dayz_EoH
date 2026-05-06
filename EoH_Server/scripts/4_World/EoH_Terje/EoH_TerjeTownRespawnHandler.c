class EoH_TerjeTownRespawnHandler : TerjeRespawnObjectHandler
{
    override bool IsObjectValid(Object object, PlayerBase player, string respawnId, TerjeXmlObject xmlObject)
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

    override bool GetPlayerSpawnPos(Object object, PlayerBase player, string respawnId, TerjeXmlObject xmlObject, out vector playerPos, out vector playerOri)
    {
        if (!IsObjectValid(object, player, respawnId, xmlObject))
            return false;

        vector relayPos = object.GetPosition();
        playerPos = EoH_TerritorySpawnBridge.SafeSpawnPosition(relayPos, 6.0);
        playerOri = "0 0 0".ToVector();
        return true;
    }

    override string GetFailMessage(Object object, PlayerBase player, string respawnId, TerjeXmlObject xmlObject)
    {
        return "Your group no longer controls this town relay.";
    }
}
