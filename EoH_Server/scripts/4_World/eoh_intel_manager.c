class EoH_IntelManager
{
    static ref EoH_IntelManager s_Instance;
    static bool s_ManualTownIntelAllowed;

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

    static void BeginManualTownIntel()
    {
        s_ManualTownIntelAllowed = true;
    }

    static void EndManualTownIntel()
    {
        s_ManualTownIntelAllowed = false;
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
        RevealTownIntel(player);
    }

    void RevealTownIntel(PlayerBase player)
    {
        if (!s_ManualTownIntelAllowed)
        {
            if (player && player.GetIdentity())
                Print("[EoH_Intel][BLOCKED] Automatic town intel trigger blocked player=" + player.GetIdentity().GetName());
            else
                Print("[EoH_Intel][BLOCKED] Automatic town intel trigger blocked no valid player");
            return;
        }

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

        string owner = GetTownOwnerLabel(town);
        int infectedCount = CountInfectedInRadius(townPos, EoH_CaptureManager.CAPTURE_RADIUS);
        int aiCount = CountAIInRadius(townPos, EoH_CaptureManager.CAPTURE_RADIUS);
        int playerCount = CountPlayersInRadius(townPos, EoH_CaptureManager.CAPTURE_RADIUS);

        string infected = BuildInfectedLevel(infectedCount);
        string humanThreat = BuildHumanThreatLevel(aiCount, playerCount, owner);
        string medical = GetWeightedMedicalOpportunity();
        string confidence = GetWeightedConfidence(distance);
        string recommendation = BuildTownRiskRecommendation(infected, humanThreat, medical);

        string body = "LOCATION: " + town + "\n";
        body += "CONTROL: " + owner + "\n";
        body += "CAPTURE RADIUS: " + EoH_CaptureManager.CAPTURE_RADIUS.ToString() + "m\n\n";
        body += "LIVE COUNTS INSIDE RADIUS:\n";
        body += "INFECTED: " + infectedCount.ToString() + "\n";
        body += "AI / BANDITS: " + aiCount.ToString() + "\n";
        body += "SURVIVORS: " + playerCount.ToString() + "\n\n";
        body += "ASSESSMENT:\n";
        body += "INFECTED PRESENCE: " + infected + "\n";
        body += "HUMAN THREAT: " + humanThreat + "\n";
        body += "MEDICAL OPPORTUNITY: " + medical + "\n";
        body += "INTEL CONFIDENCE: " + confidence + "\n\n";
        body += "RECOMMENDATION:\n" + recommendation;

        EoH_FieldReportData report = new EoH_FieldReportData();
        report.Title = "TOWN RISK REPORT";
        report.Subtitle = "Echoes of Humanity Intelligence Network";
        report.Body = body;

        EoH_FieldReportService.OpenForPlayer(player, report);

        Print("[EoH_Intel] Town risk report town=" + town + " owner=" + owner + " infectedCount=" + infectedCount.ToString() + " aiCount=" + aiCount.ToString() + " playerCount=" + playerCount.ToString() + " player=" + player.GetIdentity().GetName());
    }

    string GetTownOwnerLabel(string town)
    {
        EoH_CaptureManager cap = EoH_CaptureManager.Get();
        if (!cap)
            return "Unclaimed";

        string owner = cap.GetTownOwner(town);
        if (owner == "" || owner == "Unclaimed")
            return "Unclaimed";

        return owner;
    }

    int CountPlayersInRadius(vector townPos, float radius)
    {
        int count = 0;
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity() || !player.IsAlive())
                continue;

            if (Distance2D(player.GetPosition(), townPos) <= radius)
                count++;
        }

        return count;
    }

    int CountInfectedInRadius(vector townPos, float radius)
    {
        int count = 0;
        array<Object> objects = new array<Object>();
        vector searchPos = townPos;
        searchPos[1] = GetGame().SurfaceY(townPos[0], townPos[2]);
        GetGame().GetObjectsAtPosition3D(searchPos, radius, objects, null);

        foreach (Object obj : objects)
        {
            if (!obj)
                continue;

            ZombieBase zmb = ZombieBase.Cast(obj);
            if (zmb && zmb.IsAlive())
                count++;
        }

        return count;
    }

    int CountAIInRadius(vector townPos, float radius)
    {
        int count = 0;
        array<Object> objects = new array<Object>();
        vector searchPos = townPos;
        searchPos[1] = GetGame().SurfaceY(townPos[0], townPos[2]);
        GetGame().GetObjectsAtPosition3D(searchPos, radius, objects, null);

        foreach (Object obj : objects)
        {
            if (!obj)
                continue;

            if (IsAIThreat(obj))
                count++;
        }

        return count;
    }

    bool IsAIThreat(Object obj)
    {
        if (!obj)
            return false;

        string type = obj.GetType();
        type.ToLower();

        if (type.Contains("eai") || type.Contains("aib") || type.Contains("bandit") || type.Contains("raider"))
            return true;

        DayZPlayerImplement aiPlayer = DayZPlayerImplement.Cast(obj);
        if (aiPlayer && !PlayerBase.Cast(obj))
            return true;

        return false;
    }

    float Distance2D(vector a, vector b)
    {
        float dx = a[0] - b[0];
        float dz = a[2] - b[2];
        return Math.Sqrt((dx * dx) + (dz * dz));
    }

    string BuildInfectedLevel(int count)
    {
        if (count <= 3)
            return "Low";
        if (count <= 10)
            return "Moderate";
        if (count <= 20)
            return "High";

        return "Severe";
    }

    string BuildHumanThreatLevel(int aiCount, int playerCount, string owner)
    {
        if (aiCount >= 8 || playerCount >= 4)
            return "Heavy activity";

        if (aiCount >= 3 || playerCount >= 2)
            return "Confirmed";

        if (aiCount > 0 || playerCount > 0)
            return "Possible";

        if (owner != "Unclaimed")
            return "Possible";

        return "None reported";
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
