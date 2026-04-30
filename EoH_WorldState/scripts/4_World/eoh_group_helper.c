class EoH_GroupHelper
{
    static string GetGroupID(PlayerBase player)
    {
        if (!player)
            return "";

        ExpansionPartyData party = player.Expansion_GetParty();

        if (!party)
            return "";

        return party.GetPartyID().ToString();
    }

    static string GetGroupName(PlayerBase player)
    {
        if (!player)
            return "Unknown";

        ExpansionPartyData party = player.Expansion_GetParty();

        if (!party)
            return "No Group";

        return party.GetPartyName();
    }

    static bool IsSameGroup(PlayerBase a, PlayerBase b)
    {
        if (!a || !b)
            return false;

        return a.Expansion_GetPartyID() == b.Expansion_GetPartyID();
    }
};
