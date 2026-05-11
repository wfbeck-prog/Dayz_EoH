class EoH_ServerStatusWebhook
{
	static void NotifyOnline()
	{
		string message = "🟢 **SERVER ONLINE**\n\n";
		message += "The signal is back. Survivors may re-enter the zone.";
		SendStatus("SERVER ONLINE", message);
	}

	static void NotifyRestartWarning(int minutesRemaining)
	{
		string message = "🟡 **SERVER RESTART WARNING**\n\n";
		message += "The zone will cycle in " + minutesRemaining.ToString() + " minutes. Secure your gear, finish trades, and get clear.";
		SendStatus("RESTART WARNING", message);
	}

	static void NotifyRestartImminent()
	{
		string message = "🔴 **SERVER RESTART IMMINENT**\n\n";
		message += "The signal is collapsing. Find safety now.";
		SendStatus("RESTART IMMINENT", message);
	}

	static void NotifyCustom(string title, string body)
	{
		SendStatus(title, body);
	}

	protected static void SendStatus(string title, string message)
	{
		#ifdef EOH_DISCORD_WEBHOOK
		EoH_DiscordWebhook.Send("SERVER_STATUS", title, message);
		#else
		Print("[EoH_ServerStatusWebhook] " + title + ": " + message);
		#endif
	}
};
