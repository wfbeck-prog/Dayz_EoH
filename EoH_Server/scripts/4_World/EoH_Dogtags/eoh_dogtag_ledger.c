class EoH_DogtagLedger
{
	static void NotifyRecovered(string tagOwnerName, string recoveredByName, vector position)
	{
		string location = EoH_DogtagLedger.FormatLocation(position);
		string message = "A survivor's tag has changed hands.\\n\\n";
		message += "Tag: " + EoH_DogtagLedger.SafeName(tagOwnerName) + "\\n";
		message += "Recovered By: " + EoH_DogtagLedger.SafeName(recoveredByName) + "\\n";
		message += "Location: " + location;

		EoH_DogtagLedger.SendToDiscord("DOGTAG LEDGER", message);
	}

	static void NotifyTurnedIn(string tagOwnerName, string turnedInByName, int reward)
	{
		string message = "A tag was delivered to the ledger.\\n\\n";
		message += "Tag: " + EoH_DogtagLedger.SafeName(tagOwnerName) + "\\n";
		message += "Turned In By: " + EoH_DogtagLedger.SafeName(turnedInByName);

		if (reward > 0)
			message += "\\nReward: $" + reward.ToString();

		EoH_DogtagLedger.SendToDiscord("DOGTAG TURN-IN", message);
	}

	static void NotifyClaimedFromBody(string tagOwnerName, PlayerBase recoveredBy, vector position)
	{
		string recoveredByName = "Unknown Survivor";
		if (recoveredBy && recoveredBy.GetIdentity())
			recoveredByName = recoveredBy.GetIdentity().GetName();

		NotifyRecovered(tagOwnerName, recoveredByName, position);
	}

	protected static string SafeName(string value)
	{
		if (value == string.Empty)
			return "Unknown Survivor";

		return value;
	}

	protected static string FormatLocation(vector position)
	{
		if (position == vector.Zero)
			return "Unknown";

		return "X " + Math.Round(position[0]).ToString() + " / Z " + Math.Round(position[2]).ToString();
	}

	protected static void SendToDiscord(string title, string message)
	{
		string content = "🏷️ **THE LEDGER RECORDS ANOTHER NAME**\\n\\n" + message;

		// Preferred path: existing EoH webhook bridge can route this by key once DOGTAGS is added to EoH_Webhooks.json.
		#ifdef EOH_DISCORD_WEBHOOK
		EoH_DiscordWebhook.Send("DOGTAGS", title, content);
		#else
		Print("[EoH_DogtagLedger] " + title + ": " + message);
		#endif
	}
};
