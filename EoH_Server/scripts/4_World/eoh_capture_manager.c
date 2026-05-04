// CLEAN CONTESTED LIFECYCLE PATCH

bool wasContested = session.IsContested;

if (m_Config.PauseWhenContested && session.EnemiesNearby > 0)
    session.IsContested = true;
else
    session.IsContested = false;

vector pos = townCfg.GetRelayVector();
array<Man> players = new array<Man>();
GetGame().GetPlayers(players);

if (session.IsContested)
{
    foreach (Man man : players)
    {
        PlayerBase player = PlayerBase.Cast(man);
        if (!player || !player.GetIdentity()) continue;

        if (vector.Distance(player.GetPosition(), pos) <= townCfg.Radius * 1.5)
        {
            GetGame().RPCSingleParam(
                player,
                777003,
                new Param2<vector, string>(pos, session.TownName),
                true,
                player.GetIdentity()
            );
        }
    }
}
else if (wasContested)
{
    // contest ended → restore base marker
    foreach (Man man : players)
    {
        PlayerBase player = PlayerBase.Cast(man);
        if (!player || !player.GetIdentity()) continue;

        if (vector.Distance(player.GetPosition(), pos) <= townCfg.Radius * 1.5)
        {
            GetGame().RPCSingleParam(player, 777002, new Param1<int>(0), true, player.GetIdentity());

            GetGame().RPCSingleParam(
                player,
                777001,
                new Param3<vector, string, int>(pos, session.TownName, ARGB(120,150,150,150)),
                true,
                player.GetIdentity()
            );
        }
    }
}
