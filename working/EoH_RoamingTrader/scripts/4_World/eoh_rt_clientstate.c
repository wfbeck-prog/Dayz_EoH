class EoH_RT_ClientState
{
	private static ref EoH_RT_ClientState s_Instance;
	ref array<ref EoH_RT_NotificationData> PendingNotifications;

	void EoH_RT_ClientState()
	{
		PendingNotifications = new array<ref EoH_RT_NotificationData>();
	}

	static EoH_RT_ClientState GetInstance()
	{
		if (!s_Instance)
			s_Instance = new EoH_RT_ClientState();
		return s_Instance;
	}

	void QueueNotification(EoH_RT_NotificationData data)
	{
		if (data)
			PendingNotifications.Insert(data);
	}

	EoH_RT_NotificationData PopNotification()
	{
		if (!PendingNotifications || PendingNotifications.Count() == 0)
			return NULL;
		EoH_RT_NotificationData data = PendingNotifications.Get(0);
		PendingNotifications.Remove(0);
		return data;
	}
};
