modded class TerjePlayerConditions
{
    override bool ProcessCondition(PlayerBase player, TerjeXmlObject condition, out string displayText)
    {
        if (condition && condition.GetName() == "EoHOwnedTown")
            return EoH_ProcessOwnedTownCondition(player, condition, displayText);

        return super.ProcessCondition(player, condition, displayText);
    }

    bool EoH_ProcessOwnedTownCondition(PlayerBase player, TerjeXmlObject condition, out string displayText)
    {
        displayText = "Requires your group to own this town.";

        if (!player || !player.GetIdentity() || !condition)
            return false;

        string townName;
        if (!condition.FindAttribute("town", townName) || townName == "")
            return false;

        string playerGroupID = EoH_GroupHelper.GetGroupID(player);
        if (playerGroupID == "")
            return false;

        EoH_WorldStateTownState state = EoH_WorldStateManager.Get().GetTownState(townName);
        if (!state || state.OwnerGroupID == "")
            return false;

        bool owned = state.OwnerGroupID == playerGroupID;
        if (owned)
            displayText = "Your group controls " + townName + ".";
        else
            displayText = "Your group does not control " + townName + ".";

        if (player.GetIdentity())
            Print("[EoH_TerjeRespawn][CONDITION] town=" + townName + " player=" + player.GetIdentity().GetName() + " playerGroup=" + playerGroupID + " ownerGroup=" + state.OwnerGroupID + " owned=" + owned.ToString());

        return owned;
    }
}
