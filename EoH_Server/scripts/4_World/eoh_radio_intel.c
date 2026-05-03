class EoH_RadioIntel
{
    static ref EoH_RadioIntel s_Instance;

    void EoH_RadioIntel() {}

    static EoH_RadioIntel Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_RadioIntel();
        return s_Instance;
    }

    void BroadcastFromTower(vector pos, float radius)
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.IsAlive())
                continue;

            if (vector.Distance(player.GetPosition(), pos) > radius)
                continue;

            EoH_IntelManager.Get().RevealTownIntel(player);
        }
    }
};
