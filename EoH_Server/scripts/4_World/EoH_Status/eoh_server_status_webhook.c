class EoH_ServerStatusWebhook
{
	static void NotifyOnline()
	{
		string message = "🟢 **SERVER ONLINE**\n\n";
		message += "The signal is back. Survivors may re-enter the zone.\n\n";
		message += BuildStatusBlock();
		SendStatus("SERVER ONLINE", message);
	}

	static void NotifyStatusUpdate()
	{
		string message = "📡 **SERVER STATUS UPDATE**\n\n";
		message += BuildStatusBlock();
		SendStatus("SERVER STATUS", message);
	}

	static void NotifyRestartWarning(int minutesRemaining)
	{
		string message = "🟡 **SERVER RESTART WARNING**\n\n";
		message += "The zone will cycle in " + minutesRemaining.ToString() + " minutes. Secure your gear, finish trades, and get clear.\n\n";
		message += BuildStatusBlock();
		SendStatus("RESTART WARNING", message);
	}

	static void NotifyRestartImminent()
	{
		string message = "🔴 **SERVER RESTART IMMINENT**\n\n";
		message += "The signal is collapsing. Find safety now.\n\n";
		message += BuildStatusBlock();
		SendStatus("RESTART IMMINENT", message);
	}

	static void NotifyCustom(string title, string body)
	{
		SendStatus(title, body);
	}

	static string BuildStatusBlock()
	{
		return "Survivors Online: " + GetOnlinePlayerCount().ToString() + "\n" + "In-Game Time: " + GetInGameTimeText();
	}

	static int GetOnlinePlayerCount()
	{
		array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);
		return players.Count();
	}

	static string GetInGameTimeText()
	{
		int year;
		int month;
		int day;
		int hour;
		int minute;

		GetGame().GetWorld().GetDate(year, month, day, hour, minute);

		string hourText = hour.ToString();
		if (hour < 10)
			hourText = "0" + hourText;

		string minuteText = minute.ToString();
		if (minute < 10)
			minuteText = "0" + minuteText;

		return hourText + ":" + minuteText;
	}

	protected static void SendStatus(string title, string message)
	{
		EoH_DiscordWebhook.SendServerStatus(message);
	}
};
