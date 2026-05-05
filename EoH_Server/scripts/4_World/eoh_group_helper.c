class EoH_GroupHelper
{
    static string GetGroupID(PlayerBase player)
    {
        if (!player)
            return "";

        string partyId = player.Expansion_GetPartyID().ToString();
        if (partyId != "" && partyId != "0")
            return "EXP_" + partyId;

        ExpansionPartyData party = player.Expansion_GetParty();
        if (party)
            return "EXP_" + party.GetPartyID().ToString();

        if (player.GetIdentity())
            return "SOLO_" + player.GetIdentity().GetId();

        return "";
    }

    static string GetGroupName(PlayerBase player)
    {
        if (!player)
            return "Unknown";

        ExpansionPartyData party = player.Expansion_GetParty();
        if (party && party.GetPartyName() != "")
            return party.GetPartyName();

        if (player.GetIdentity())
            return "Solo - " + player.GetIdentity().GetName();

        return "Solo Survivor";
    }

    static bool IsExpansionGroup(PlayerBase player)
    {
        if (!player)
            return false;

        string partyId = player.Expansion_GetPartyID().ToString();
        if (partyId != "" && partyId != "0")
            return true;

        return player.Expansion_GetParty() != null;
    }

    static bool IsSameGroup(PlayerBase a, PlayerBase b)
    {
        if (!a || !b)
            return false;

        string aGroup = GetGroupID(a);
        string bGroup = GetGroupID(b);

        if (aGroup == "" || bGroup == "")
            return false;

        return aGroup == bGroup;
    }
};
