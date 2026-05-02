class EoH_RT_RPC
{
	static const int ADD_OR_UPDATE_MARKER = 0x454F4830;
	static const int REMOVE_MARKER = 0x454F4831;
	static const int NOTIFICATION = 0x454F4832;
	static const int OPEN_TRADER = 0x454F4833;
	static const int ADD_OR_UPDATE_KEYROOM_MARKER = 0x454F4834;
static const int REMOVE_KEYROOM_MARKER = 0x454F4835;
};

class EoH_RT_NotificationData
{
	string Title;
	string Message;
	string Style;

	void EoH_RT_NotificationData(string title = "", string message = "", string style = "popup")
	{
		Title = title;
		Message = message;
		Style = style;
	}
};

class EoH_RT_MarkerData
{
	string TraderId;
	string Label;
	vector Position;

	int Color;
	int Pulse;
	string Icon;
	int Is3D;

	void EoH_RT_MarkerData(string traderId = "", string label = "", vector position = "0 0 0")
	{
		TraderId = traderId;
		Label = label;
		Position = position;

		Color = ARGB(255, 0, 200, 0);
		Pulse = 0;
		Icon = "Trader";
		Is3D = 1;
	}
};