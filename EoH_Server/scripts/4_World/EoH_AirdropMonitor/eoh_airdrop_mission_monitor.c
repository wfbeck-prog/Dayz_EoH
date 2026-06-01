class EoH_AirdropMissionMonitorRecord
{
    string MissionName;
    string Category;
    string Region;
    string Status;
    int LastPostedAt;

    void EoH_AirdropMissionMonitorRecord()
    {
        MissionName = "";
        Category = "";
        Region = "";
        Status = "";
        LastPostedAt = 0;
    }
};

class EoH_AirdropMissionMonitor
{
    protected static ref EoH_AirdropMissionMonitor s_Instance;
    protected ref map<string, ref EoH_AirdropMissionMonitorRecord> m_Posted;

    void EoH_AirdropMissionMonitor()
    {
        m_Posted = new map<string, ref EoH_AirdropMissionMonitorRecord>();
    }

    static EoH_AirdropMissionMonitor Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_AirdropMissionMonitor();

        return s_Instance;
    }

    void OnExpansionAirdropMissionStarted(string missionName, string dropLocationName = "", vector dropPosition = "0 0 0")
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!IsAirdropMission(missionName))
            return;

        string category = ResolveCategory(missionName, dropLocationName);
        string region = ResolveRegion(missionName, dropLocationName);
        string threat = ResolveThreatLevel(category, region);
        string key = missionName + "_started_" + region;

        if (WasRecentlyPosted(key))
            return;

        MarkPosted(key, missionName, category, region, "inbound");

        string title = "📦 AIRDROP SIGNAL DETECTED";
        string body = "Category: " + category + "\n";
        body += "Region: " + region + "\n";
        body += "Status: inbound\n";
        body += "Threat level: " + threat + "\n\n";
        body += ResolveFlavor(category, false);

        EoH_DiscordWebhook.SendAirdropTracking(region, "inbound", threat, IsHighValue(category, region));
        EoH_DiscordWebhook.SendRadioBroadcast(title, body);
    }

    void OnExpansionAirdropLanded(string missionName, string dropLocationName = "", vector dropPosition = "0 0 0")
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!IsAirdropMission(missionName))
            return;

        string category = ResolveCategory(missionName, dropLocationName);
        string region = ResolveRegion(missionName, dropLocationName);
        string threat = ResolveThreatLevel(category, region);
        string key = missionName + "_landed_" + region;

        if (WasRecentlyPosted(key))
            return;

        MarkPosted(key, missionName, category, region, "landed");

        string title = "📦 AIRDROP IMPACT CONFIRMED";
        string body = "Category: " + category + "\n";
        body += "Region: " + region + "\n";
        body += "Status: landed\n";
        body += "Threat level: " + threat + "\n\n";
        body += ResolveFlavor(category, true);

        EoH_DiscordWebhook.SendAirdropTracking(region, "landed", threat, IsHighValue(category, region));
    }

    bool IsAirdropMission(string missionName)
    {
        if (missionName == "")
            return false;

        return missionName.Contains("Airdrop") || missionName.Contains("Drugs_") || missionName.Contains("Medical_") || missionName.Contains("BaseBuilding_");
    }

    string ResolveCategory(string missionName, string dropLocationName)
    {
        string source = missionName + " " + dropLocationName;

        if (source.Contains("Drugs"))
            return "Drugs";

        if (source.Contains("Medical"))
            return "Medical";

        if (source.Contains("BaseBuilding"))
            return "Base Building";

        return "Supply";
    }

    string ResolveRegion(string missionName, string dropLocationName)
    {
        if (dropLocationName != "")
        {
            string clean = dropLocationName;
            clean.Replace("Drugs ", "");
            clean.Replace("Medical ", "");
            clean.Replace("BaseBuilding ", "");
            clean.Replace("Base Building ", "");
            clean.Replace("_", " ");
            clean.Replace("-", " ");
            return clean;
        }

        string region = missionName;
        region.Replace("Airdrop_", "");
        region.Replace("Drugs_", "");
        region.Replace("Medical_", "");
        region.Replace("BaseBuilding_", "");
        region.Replace("_", " ");
        region.Replace("-", " ");

        if (region == "")
            region = "Unknown Region";

        return region;
    }

    string ResolveThreatLevel(string category, string region)
    {
        if (region.Contains("NWAF") || region.Contains("Tisy") || region.Contains("Balota") || region.Contains("NEAF"))
            return "extreme";

        if (category == "Drugs")
            return "extreme";

        if (category == "Base Building")
            return "high";

        return "high";
    }

    bool IsHighValue(string category, string region)
    {
        if (category == "Drugs")
            return true;

        if (region.Contains("NWAF") || region.Contains("Tisy") || region.Contains("Balota") || region.Contains("NEAF"))
            return true;

        return false;
    }

    string ResolveFlavor(string category, bool landed)
    {
        if (category == "Drugs")
        {
            if (landed)
                return "Smuggler-grade cargo has touched down. Expect armed contact and black-market movement.";
            return "Smuggler-grade cargo has entered the relay net. Expect armed contact.";
        }

        if (category == "Medical")
        {
            if (landed)
                return "Emergency medical cargo is now on the ground. Terje field supplies may be present.";
            return "Emergency medical cargo is in the air. Terje field supplies may be present.";
        }

        if (category == "Base Building")
        {
            if (landed)
                return "Construction-grade supplies have landed. Territory groups may move fast.";
            return "Construction-grade supplies have been detected near a contested route.";
        }

        if (landed)
            return "A supply container has touched down. Expect survivor contact.";

        return "A military supply beacon has been detected by the old relay network.";
    }

    protected bool WasRecentlyPosted(string key)
    {
        if (!m_Posted || !m_Posted.Contains(key))
            return false;

        EoH_AirdropMissionMonitorRecord rec = m_Posted.Get(key);
        if (!rec)
            return false;

        int now = GetGame().GetTime();
        return now - rec.LastPostedAt < 1800000;
    }

    protected void MarkPosted(string key, string missionName, string category, string region, string status)
    {
        EoH_AirdropMissionMonitorRecord rec = new EoH_AirdropMissionMonitorRecord();
        rec.MissionName = missionName;
        rec.Category = category;
        rec.Region = region;
        rec.Status = status;
        rec.LastPostedAt = GetGame().GetTime();
        m_Posted.Set(key, rec);
    }
};
