class EoH_IntelManager
{
    static ref EoH_IntelManager s_Instance;
    ref EoH_IntelConfig m_Config;

    void EoH_IntelManager()
    {
        m_Config = new EoH_IntelConfig();
    }

    static EoH_IntelManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_IntelManager();
        return s_Instance;
    }

    void RevealTownIntel(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        EoH_CaptureManager cap = EoH_CaptureManager.Get();
        array<string> towns = cap.GetAllTownNames();

        foreach (string town : towns)
        {
            string owner = cap.GetTownOwner(town);

            EoH_TownMarkerData data = new EoH_TownMarkerData();
            data.Id = "EoH_INTEL_" + town;
            data.Name = town;
            data.Position = EoH_TownMarkerManager.GetTownPosition(town);

            if (owner != "")
            {
                data.Owner = owner;
                data.Color = ARGB(255, 0, 200, 0);
            }
            else
            {
                data.Owner = "Unclaimed";
                data.Color = ARGB(120, 150, 150, 150);
            }

            EoH_TownMarkerManager.SendMarkerToPlayer(player, data);
        }

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ClearTownIntel, m_Config.RevealDurationSeconds * 1000, false, player);
    }

    void RevealHighValuePlayers(PlayerBase player)
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase target = PlayerBase.Cast(man);
            if (!target || !target.GetIdentity() || !target.IsAlive())
                continue;

            int exp = EoH_DT_TerjeAdapter.GetBestSkillScore(target);
            if (exp < 2000)
                continue;

            EoH_TownMarkerData data = new EoH_TownMarkerData();
            data.Id = "EoH_INTEL_PLAYER_" + target.GetIdentity().GetId();
            data.Name = "High Value Target";
            data.Position = target.GetPosition();
            data.Color = ARGB(255, 255, 50, 50);

            EoH_TownMarkerManager.SendMarkerToPlayer(player, data);
        }

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ClearPlayerIntel, m_Config.RevealDurationSeconds * 1000, false, player);
    }

    void ClearTownIntel(PlayerBase player)
    {
        EoH_CaptureManager cap = EoH_CaptureManager.Get();
        array<string> towns = cap.GetAllTownNames();

        foreach (string town : towns)
        {
            EoH_TownMarkerManager.RemoveMarkerFromPlayer(player, "EoH_INTEL_" + town);
        }
    }

    void ClearPlayerIntel(PlayerBase player)
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase target = PlayerBase.Cast(man);
            if (!target || !target.GetIdentity())
                continue;

            EoH_TownMarkerManager.RemoveMarkerFromPlayer(player, "EoH_INTEL_PLAYER_" + target.GetIdentity().GetId());
        }
    }
};
