class EoH_DiscordWebhookConfig
{
    int EnableTraderIntelWebhook;
    string TraderIntelWebhook;

    void EoH_DiscordWebhookConfig()
    {
        EnableTraderIntelWebhook = 0;
        TraderIntelWebhook = "";
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

        string content = "📡 **TRADER INTEL DECODED**\n";
        content += "Survivor: " + name + "\n";
        content += "Result: " + result + "\n";
        content += "Someone is listening to the old trade frequencies.";

        Send(cfg.TraderIntelWebhook, content);
    }

    static void Send(string webhookUrl, string content)
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

        string payload = "{\"content\":\"" + EscapeJson(content) + "\"}";
        ctx.POST(null, "", payload);
        Print("[EoH_Discord] Trader intel webhook sent.");
    }

    static string EscapeJson(string value)
    {
        value.Replace("\\", "\\\\");
        value.Replace("\"", "\\\"");
        value.Replace("\n", "\\n");
        value.Replace("\r", "");
        return value;
    }
}
