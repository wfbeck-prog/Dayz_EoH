modded class MissionGameplay
{
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		EoH_RT_ProcessNotifications();
	}

	void EoH_RT_ProcessNotifications()
	{
		EoH_RT_ClientState state = EoH_RT_ClientState.GetInstance();
		if (!state)
			return;

		EoH_RT_NotificationData notif = state.PopNotification();
		if (!notif)
			return;

		if (notif.Style == "chat")
		{
			GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCSystem, "", notif.Message, ""));
		}
		else
		{
			NotificationSystem.AddNotificationExtended(8.0, notif.Title, notif.Message, "set:dayz_gui image:icon_connect");
		}
	}
};
