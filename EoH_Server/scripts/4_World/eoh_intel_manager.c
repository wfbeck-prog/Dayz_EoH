class EoH_IntelManager
{
    static ref EoH_IntelManager s_Instance;

    ref map<string, vector> m_IntelLocations;
    ref map<string, int> m_PlayerIntelUses;

    void EoH_IntelManager()
    {
        m_IntelLocations = new map<string, vector>();
        m_PlayerIntelUses = new map<string, int>();
        InitIntel();
    }

    static EoH_IntelManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_IntelManager();

        return s_Instance;
    }

    void InitIntel()
    {
        m_IntelLocations.Clear();

        EoH_CaptureManager cap = EoH_CaptureManager.Get();
        if (!cap)
            return;

        array<string> towns = cap.GetAllTownNames();
        foreach (string town : towns)
        {
            m_IntelLocations.Set(town, cap.GetTownPos(town));
        }
    }

    void ResetPlayerIntelUsage(string playerId)
    {
        if (playerId == "" || !m_PlayerIntelUses)
            return;

        m_PlayerIntelUses.Remove(playerId);
    }

    void RevealIntel(PlayerBase player)
    {
        RevealTownRiskReport(player);
    }

    void RevealTownIntel(PlayerBase player)
    {
        RevealTownRiskReport(player);
    }

    void RevealTownRiskReport(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        TrackIntelUse(player);

        if (!m_IntelLocations || m_IntelLocations.Count() == 0)
            InitIntel();

        if (!m_IntelLocations || m_IntelLocations.Count() == 0)
        {
            EoH_Notifications.SendToPlayer(player, "TOWN RISK REPORT", "The report is too damaged to read.");
            Print("[EoH_Intel] Town risk report failed: no town intel locations player=" + player.GetIdentity().GetName());
            return;
        }

        string town = GetNearestKnownTown(player.GetPosition());
        vector townPos = m_IntelLocations.Get(town);
        float distance = vector.Distance(player.GetPosition(), townPos);

        string infected = GetWeightedRiskLevel();
        string humanThreat = GetWeightedHumanThreat();
        string medical = GetWeightedMedicalOpportunity();
        string confidence = GetWeightedConfidence(distance);
        string recommendation = BuildTownRiskRecommendation(infected, humanThreat, medical);

        string body = "LOCATION: " + town + "\n\n";
        body += "INFECTED PRESENCE: " + infected + "\n";
        body += "HUMAN THREAT: " + humanThreat + "\n";
        body += "MEDICAL OPPORTUNITY: " + medical + "\n";
        body += "INTEL CONFIDENCE: " + confidence + "\n\n";
        body += "RECOMMENDATION:\n" + recommendation;

        EoH_FieldReportData report = new EoH_FieldReportData(
            "TOWN RISK REPORT",
            "Echoes of Humanity Intelligence Network",
            body
        );

        EoH_FieldReportService.OpenForPlayer(player, report);

        Print("[EoH_Intel] Town risk report town=" + town + " infected=" + infected + " humanThreat=" + humanThreat + " medical=" + medical + " confidence=" + confidence + " player=" + player.GetIdentity().GetName());
    }

    string GetNearestKnownTown(vector playerPos)
    {
        string bestTown = "Unknown Settlement";
        float bestDistance = 999999.0;

        foreach (string name, vector pos : m_IntelLocations)
        {
            float distance = vector.Distance(playerPos, pos);
            if (distance < bestDistance)
            {
                bestDistance = distance;
                bestTown = name;
            }
        }

        return bestTown;
    }

    string GetWeightedRiskLevel()
    {
        int roll = Math.RandomIntInclusive(1, 100);

        if (roll <= 20)
            return "Low";
        if (roll <= 50)
            return "Moderate";
        if (roll <= 82)
            return "High";

        return "Severe";
    }

    string GetWeightedHumanThreat()
    {
        int roll = Math.RandomIntInclusive(1, 100);

        if (roll <= 25)
            return "None reported";
        if (roll <= 58)
            return "Possible";
        if (roll <= 86)
            return "Confirmed";

        return "Heavy activity";
    }

    string GetWeightedMedicalOpportunity()
    {
        int roll = Math.RandomIntInclusive(1, 100);

        if (roll <= 25)
            return "Poor";
        if (roll <= 60)
            return "Moderate";
        if (roll <= 88)
            return "Good";

        return "High-value medical lead";
    }

    string GetWeightedConfidence(float distance)
    {
        int roll = Math.RandomIntInclusive(1, 100);

        if (distance > 6000)
            roll = roll + 15;

        if (roll <= 20)
            return "Outdated";
        if (roll <= 52)
            return "Unverified";
        if (roll <= 84)
            return "Reliable";

        return "Recently confirmed";
    }

    string BuildTownRiskRecommendation(string infected, string humanThreat, string medical)
    {
        if (infected == "Severe" || humanThreat == "Heavy activity")
            return "Do not enter alone. Move quiet, avoid the main road, and carry medical supplies.";

        if (infected == "High" || humanThreat == "Confirmed")
            return "High-risk approach. Scout first, avoid gunfire, and keep an exit route open.";

        if (medical == "High-value medical lead" || medical == "Good")
            return "Search clinics, aid stations, and survivor shelters. Medical supplies may still be recoverable.";

        if (infected == "Low" && humanThreat == "None reported")
            return "Low activity reported. Good for quiet scavenging, food, water, and basic supplies.";

        return "Proceed carefully. Conditions are unstable and the report may not reflect current movement.";
    }

    void RevealTraderIntel(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        TrackIntelUse(player);

        bool revealed = EoH_RT_TraderManager.Get().RevealNearestHiddenTraderToPlayer(player);

        EoH_DiscordWebhook.SendTraderIntel(player, revealed);

        if (revealed)
            EoH_Notifications.SendToPlayer(player, "TRADER INTEL", "A roaming trader signal was marked.");
        else
            EoH_Notifications.SendToPlayer(player, "TRADER INTEL", "No hidden trader signal was found.");

        Print("[EoH_Intel] Trader intel used revealed=" + revealed.ToString() + " player=" + player.GetIdentity().GetName());
    }

    void RevealCBDIntel(PlayerBase player)
    {
        RevealLootCacheIntel(player);
    }

    void RevealLootCacheIntel(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        TrackIntelUse(player);

        bool revealed = EoH_DNACacheIntelManager.Get().RevealNearestCacheToPlayer(player);

        if (revealed)
            EoH_Notifications.SendToPlayer(player, "LOOT CACHE INTEL", "DNA cache coordinates were decoded and broadcast.");
        else
            EoH_Notifications.SendToPlayer(player, "LOOT CACHE INTEL", "No DNA cache signal could be triangulated.");

        Print("[EoH_LootCacheIntel] DNA cache intel used revealed=" + revealed.ToString() + " player=" + player.GetIdentity().GetName());
    }

    void TrackIntelUse(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        string playerId = player.GetIdentity().GetId();
        int used = 0;
        m_PlayerIntelUses.Find(playerId, used);
        m_PlayerIntelUses.Set(playerId, used + 1);
        Print("[EoH_Intel] Track use player=" + player.GetIdentity().GetName() + " count=" + (used + 1).ToString());
    }
};
