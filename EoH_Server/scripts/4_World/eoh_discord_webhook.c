class EoH_DiscordWebhookConfig
{
    int EnableTraderIntelWebhook;
    string TraderIntelWebhook;
    int EnableKillFeedWebhook;
    string KillFeedWebhook;
    int EnableBunkerWebhook;
    string BunkerWebhook;
    int EnableDogtagWebhook;
    string DogtagWebhook;
    int EnableServerStatusWebhook;
    string ServerStatusWebhook;
    int EnableHighValueKillWebhook;
    string HighValueKillWebhook;

    int EnableAirdropWebhook;
    string AirdropWebhook;
    int EnableBlackMarketWebhook;
    string BlackMarketWebhook;
    int EnableAIActivityWebhook;
    string AIActivityWebhook;
    int EnableTerritoryWebhook;
    string TerritoryWebhook;
    int EnableConvoyWebhook;
    string ConvoyWebhook;
    int EnableQuestWebhook;
    string QuestWebhook;
    int EnableRadioWebhook;
    string RadioWebhook;

    void EoH_DiscordWebhookConfig()
    {
        EnableTraderIntelWebhook = 0;
        TraderIntelWebhook = "";
        EnableKillFeedWebhook = 0;
        KillFeedWebhook = "";
        EnableBunkerWebhook = 0;
        BunkerWebhook = "";
        EnableDogtagWebhook = 0;
        DogtagWebhook = "";
        EnableServerStatusWebhook = 0;
        ServerStatusWebhook = "";
        EnableHighValueKillWebhook = 0;
        HighValueKillWebhook = "";

        EnableAirdropWebhook = 0;
        AirdropWebhook = "";
        EnableBlackMarketWebhook = 0;
        BlackMarketWebhook = "";
        EnableAIActivityWebhook = 0;
        AIActivityWebhook = "";
        EnableTerritoryWebhook = 0;
        TerritoryWebhook = "";
        EnableConvoyWebhook = 0;
        ConvoyWebhook = "";
        EnableQuestWebhook = 0;
        QuestWebhook = "";
        EnableRadioWebhook = 0;
        RadioWebhook = "";
    }
}

class EoH_DiscordWebhookPayload
{
    string content;

    void EoH_DiscordWebhookPayload(string message = "")
    {
        content = message;
    }
}

class EoH_DiscordWebhook
{
    protected static ref EoH_DiscordWebhookConfig s_Config;
    protected static const string EOH_WEBHOOK_DIR = "$profile:EoH\\Webhooks";
    protected static const string EOH_WEBHOOK_FILE = "$profile:EoH\\Webhooks\\EoH_Webhooks.json";

    static EoH_DiscordWebhookConfig GetConfig()
    {
        if (!s_Config)
            LoadConfig();

        return s_Config;
    }

    static void LoadConfig()
    {
        MakeDirectory("$profile:EoH");
        MakeDirectory(EOH_WEBHOOK_DIR);

        if (FileExist(EOH_WEBHOOK_FILE))
        {
            JsonFileLoader<EoH_DiscordWebhookConfig>.JsonLoadFile(EOH_WEBHOOK_FILE, s_Config);
        }

        if (!s_Config)
            s_Config = new EoH_DiscordWebhookConfig();

        JsonFileLoader<EoH_DiscordWebhookConfig>.JsonSaveFile(EOH_WEBHOOK_FILE, s_Config);
    }

    static void SendTraderIntel(PlayerBase player, bool revealed)
    {
        EoH_DiscordWebhookConfig cfg = GetConfig();
        if (!cfg || cfg.EnableTraderIntelWebhook != 1 || cfg.TraderIntelWebhook == "")
            return;

        string name = "Unknown Survivor";
        if (player && player.GetIdentity())
            name = player.GetIdentity().GetName();

        string result = "Hidden roaming trader signal was found.";
        if (!revealed)
            result = "No unrevealed roaming trader signal was available.";

        string content = "TRADER INTEL DECODED\n";
        content += "Survivor: " + name + "\n";
        content += "Result: " + result + "\n";
        content += "Someone is listening to the old trade frequencies.";

        Send(cfg.TraderIntelWebhook, content, "Trader intel webhook sent.");
    }

    static void SendDogtagLedger(string content)
    {
        EoH_DiscordWebhookConfig cfg = GetConfig();
        if (!cfg || cfg.EnableDogtagWebhook != 1 || cfg.DogtagWebhook == "")
            return;

        Send(cfg.DogtagWebhook, content, "Dogtag ledger webhook sent.");
    }

    static void SendServerStatus(string content)
    {
        EoH_DiscordWebhookConfig cfg = GetConfig();
        if (!cfg || cfg.EnableServerStatusWebhook != 1 || cfg.ServerStatusWebhook == "")
            return;

        Send(cfg.ServerStatusWebhook, content, "Server status webhook sent.");
    }

    static void SendRadioBroadcast(string title, string body)
    {
        EoH_DiscordWebhookConfig cfg = GetConfig();
        if (!cfg || cfg.EnableRadioWebhook != 1 || cfg.RadioWebhook == "")
            return;

        Send(cfg.RadioWebhook, BuildMessage(title, body), "Radio broadcast webhook sent.");
    }

    static void SendKillFeed(string victimName, string killerName, string weaponName, float distance)
    {
        EoH_DiscordWebhookConfig cfg = GetConfig();
        if (!cfg || cfg.EnableKillFeedWebhook != 1 || cfg.KillFeedWebhook == "")
            return;

        string content = "EoH KILL FEED\n";
        content += "Victim: " + victimName + "\n";
        content += "Killer: " + killerName + "\n";

        if (weaponName != "")
            content += "Weapon: " + weaponName + "\n";

        if (distance > 0)
            content += "Distance: " + Math.Round(distance).ToString() + "m\n";

        content += "Another echo fades from Chernarus.";

        Send(cfg.KillFeedWebhook, content, "Kill feed webhook sent.");
    }

    static void SendHighValueKill(string victimName, string killerName, string weaponName, float distance, string reason)
    {
        EoH_DiscordWebhookConfig cfg = GetConfig();
        if (!cfg || cfg.EnableHighValueKillWebhook != 1 || cfg.HighValueKillWebhook == "")
            return;

        string content = "🔴 THE RED LEDGER HAS BEEN UPDATED\n\n";
        content += "A priority kill has been recorded in the zone.\n\n";
        content += "Killer: " + killerName + "\n";
        content += "Victim: " + victimName + "\n";

        if (weaponName != "")
            content += "Weapon: " + weaponName + "\n";

        if (distance > 0)
            content += "Distance: " + Math.Round(distance).ToString() + "m\n";

        if (reason != "")
            content += "Reason: " + reason + "\n";

        content += "\nThe old world keeps score, even when the dead cannot.";

        Send(cfg.HighValueKillWebhook, content, "High value kill webhook sent.");
    }

    static void SendBunkerOpened(PlayerBase player)
    {
        EoH_DiscordWebhookConfig cfg = GetConfig();
        if (!cfg || cfg.EnableBunkerWebhook != 1 || cfg.BunkerWebhook == "")
            return;

        string name = "Unknown Survivor";
        if (player && player.GetIdentity())
            name = player.GetIdentity().GetName();

        string content = "BUNKER OPENED\n";
        content += "Survivor: " + name + "\n";
        content += "The underground signal has awakened. Endgame loot is now in play. Expect heavy movement toward the bunker.";

        Send(cfg.BunkerWebhook, content, "Bunker webhook sent.");
    }

    static void SendAirdropTracking(string regionName, string status, string threatLevel = "high", bool alsoRadio = false)
    {
        EoH_DiscordWebhookConfig cfg = GetConfig();
        if (!cfg)
            return;

        string title = "📦 AIRDROP SIGNAL DETECTED";
        if (status == "landed" || status == "impact" || status == "spawned")
            title = "📦 AIRDROP IMPACT CONFIRMED";

        string body = "A military supply beacon has been picked up by the old relay network.\n";
        if (regionName != "")
            body += "Approximate region: " + regionName + "\n";
        if (status != "")
            body += "Status: " + status + "\n";
        if (threatLevel != "")
            body += "Threat level: " + threatLevel + "\n";
        body += "Expect hostile movement and survivor contact.";

        if (cfg.EnableAirdropWebhook == 1 && cfg.AirdropWebhook != "")
            Send(cfg.AirdropWebhook, BuildMessage(title, body), "Airdrop tracking webhook sent.");

        if (alsoRadio && cfg.EnableRadioWebhook == 1 && cfg.RadioWebhook != "")
            Send(cfg.RadioWebhook, BuildMessage(title, body), "Airdrop radio webhook sent.");
    }

    static void SendBlackMarketRumor(string title, string body)
    {
        EoH_DiscordWebhookConfig cfg = GetConfig();
        if (!cfg || cfg.EnableBlackMarketWebhook != 1 || cfg.BlackMarketWebhook == "")
            return;

        Send(cfg.BlackMarketWebhook, BuildMessage(title, body), "Black market webhook sent.");
    }

    static void SendAIActivity(string title, string body)
    {
        EoH_DiscordWebhookConfig cfg = GetConfig();
        if (!cfg || cfg.EnableAIActivityWebhook != 1 || cfg.AIActivityWebhook == "")
            return;

        Send(cfg.AIActivityWebhook, BuildMessage(title, body), "AI activity webhook sent.");
    }

    static void SendTerritoryConflict(string title, string body)
    {
        EoH_DiscordWebhookConfig cfg = GetConfig();
        if (!cfg || cfg.EnableTerritoryWebhook != 1 || cfg.TerritoryWebhook == "")
            return;

        Send(cfg.TerritoryWebhook, BuildMessage(title, body), "Territory conflict webhook sent.");
    }

    static void SendConvoySighting(string title, string body)
    {
        EoH_DiscordWebhookConfig cfg = GetConfig();
        if (!cfg || cfg.EnableConvoyWebhook != 1 || cfg.ConvoyWebhook == "")
            return;

        Send(cfg.ConvoyWebhook, BuildMessage(title, body), "Convoy sighting webhook sent.");
    }

    static void SendQuestContract(string title, string body)
    {
        EoH_DiscordWebhookConfig cfg = GetConfig();
        if (!cfg || cfg.EnableQuestWebhook != 1 || cfg.QuestWebhook == "")
            return;

        Send(cfg.QuestWebhook, BuildMessage(title, body), "Quest contract webhook sent.");
    }

    protected static string BuildMessage(string title, string body)
    {
        string content = title;
        if (body != "")
            content += "\n" + body;
        return content;
    }

    static void Send(string webhookUrl, string content, string logMessage = "Webhook sent.")
    {
        if (webhookUrl == "" || content == "")
            return;

        RestContext ctx = GetRestApi().GetRestContext(webhookUrl);
        if (!ctx)
        {
            Print("[EoH_Discord][WARN] Failed to create REST context.");
            return;
        }

        ctx.SetHeader("application/json");

        EoH_DiscordWebhookPayload payloadObject = new EoH_DiscordWebhookPayload(content);
        string payload;
        JsonSerializer serializer = new JsonSerializer();
        serializer.WriteToString(payloadObject, false, payload);

        ctx.POST(null, "", payload);
        Print("[EoH_Discord] " + logMessage);
    }
}
