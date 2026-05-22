modded class TerjePlayerConditions
{
    override bool ProcessCondition(PlayerBase player, TerjeXmlObject condition, out string displayText)
    {
        if (condition && condition.GetName() == "EoHOwnedTown")
        {
            return EoH_ProcessOwnedTownCondition(player, condition, displayText);
        }

        return super.ProcessCondition(player, condition, displayText);
    }

    bool EoH_ProcessOwnedTownCondition(PlayerBase player, TerjeXmlObject condition, out string displayText)
    {
        displayText = "Requires your group to own this town.";

        if (!player || !player.GetIdentity() || !condition)
            return false;

        string townName = condition.GetAttribute("town");
        if (townName == "")
            return false;

        string playerGroupID = EoH_GroupHelper.GetGroupID(player);
        if (playerGroupID == "")
            return false;

        EoH_WorldStateTownState state = EoH_WorldStateManager.Get().GetTownState(townName);
        if (!state)
            return false;

        if (state.OwnerGroupID == "")
            return false;

        if (player.GetIdentity())
        {
            Print("[EoH_TerjeRespawn][CONDITION] town=" + townName + " player=" + player.GetIdentity().GetName() + " playerGroup=" + playerGroupID + " ownerGroup=" + state.OwnerGroupID);
        }

        if (state.OwnerGroupID == playerGroupID)
            return true;

        return false;
    }
}
