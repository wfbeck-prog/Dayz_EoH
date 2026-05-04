class EoH_GroupHelper
{
    static ExpansionGroup GetGroup(PlayerBase player)
    {
        ExpansionPlayerBase expPlayer = ExpansionPlayerBase.Cast(player);
        if (!expPlayer)
            return null;

        return expPlayer.GetGroup();
    }

    static string GetGroupID(PlayerBase player)
    {
        ExpansionGroup group = GetGroup(player);
        if (!group)
            return "";

        return group.GetGroupUID();
    }
};
