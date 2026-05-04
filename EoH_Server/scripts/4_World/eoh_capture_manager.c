// ONLY showing modified sections for clarity

// --- inside UpdatePresence() ---
if (m_Config.PauseWhenContested && session.EnemiesNearby > 0)
    session.IsContested = true;

if (session.IsContested)
{
    // 🔥 NEW: send flashing contested marker (trader-style system)
    vector pos = townCfg.GetRelayVector();

    array<Man> players = new array<Man>();
    GetGame().GetPlayers(players);

    foreach (Man man : players)
    {
        PlayerBase player = PlayerBase.Cast(man);
        if (!player || !player.GetIdentity())
            continue;

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
