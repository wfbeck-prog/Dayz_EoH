class EoH_DiscordWebhookConfig
{
    int EnableTraderIntelWebhook;
    string TraderIntelWebhook;
    int EnableKillFeedWebhook;
    string KillFeedWebhook;
    int EnableBunkerWebhook;
    string BunkerWebhook;

    void EoH_DiscordWebhookConfig()
    {
        EnableTraderIntelWebhook = 0;
        TraderIntelWebhook = "";
        EnableKillFeedWebhook = 0;
        KillFeedWebhook = "";
        EnableBunkerWebhook = 0;
        BunkerWebhook = "";
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
        {
            s_Config = new EoH_DiscordWebhookConfig();
            JsonFileLoader<EoH_DiscordWebhookConfig>.JsonSaveFile(EOH_WEBHOOK_FILE, s_Config);
            Print("[EoH_Discord] Created webhook config at " + EOH_WEBHOOK_FILE);
        }
    }

    static void SendTraderIntel(PlayerBase player, bool revealed)
    {
        EoH_DiscordWebhookConfig cfg = GetConfig();
        if (!cfg || cfg.EnableTraderIntelWebhook != 1 || cfg.TraderIntelWebhook == "")
            return;

        string name = "Unknown Survivor";
        if (player && player.GetIdentity())
            name = player.GetIdentity().GetName();

        string result = "No hidden roaming trader signal was found.";
        if (revealed)
            result = "A roaming trader signal was decoded and marked.";

        string content = "TRADER INTEL DECODED\n";
        content += "Survivor: " + name + "\n";
        content += "Result: " + result + "\n";
        content += "Someone is listening to the old trade frequencies.";

        Send(cfg.TraderIntelWebhook, content, "Trader intel webhook sent.");
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
