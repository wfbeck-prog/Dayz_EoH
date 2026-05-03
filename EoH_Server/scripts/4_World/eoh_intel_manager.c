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

    void RevealIntel(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        EoH_CaptureManager cap = EoH_CaptureManager.Get();
        array<string> towns = cap.GetAllTownNames();

        foreach (string town : towns)
        {
            string owner = cap.GetTownOwner(town);

            EoH_TownMarkerData data = new EoH_TownMarkerData();
            data.Id = EoH_TownMarkerManager.GetMarkerId(town);
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

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ClearIntel, m_Config.RevealDurationSeconds * 1000, false, player);
    }

    void ClearIntel(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        EoH_TownMarkerManager.ResetPlayerMarkers(player);
    }
};
