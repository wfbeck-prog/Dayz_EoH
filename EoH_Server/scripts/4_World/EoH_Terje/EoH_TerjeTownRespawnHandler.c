class EoH_TerjeTownRespawnHandler : TerjeRespawnObjectHandler
{
    override bool IsObjectValid(Object object, PlayerBase player, string respawnId, TerjeXmlObject xmlObject)
    {
        if (!object || !player)
            return false;

        string town = ResolveTownName(object, xmlObject);
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

        string town = ResolveTownName(object, xmlObject);
        vector townPos = EoH_CaptureManager.Get().GetTownPos(town);
        if (townPos == "0 0 0".ToVector())
            return false;

        playerPos = EoH_TerritorySpawnBridge.SafeSpawnPosition(townPos, 12.0);
        playerOri = "0 0 0".ToVector();
        return true;
    }

    override string GetFailMessage(Object object, PlayerBase player, string respawnId, TerjeXmlObject xmlObject)
    {
        return "Your group no longer controls this town.";
    }

    string ResolveTownName(Object object, TerjeXmlObject xmlObject)
    {
        string town = "";

        if (xmlObject && xmlObject.FindAttribute("town", town) && town != "")
            return town;

        EoH_TownRespawnTerminal terminal = EoH_TownRespawnTerminal.Cast(object);
        if (terminal)
            return terminal.GetEoHTownName();

        return "";
    }
}
