class EoH_RT_TraderManager
{
	private static ref EoH_RT_TraderManager s_Instance;
	protected ref EoH_RT_GlobalConfig m_Config;
	protected ref map<string, ref EoH_RT_TraderRuntime> m_Runtimes;
	protected ref map<string, bool> m_Revealed;

	static EoH_RT_TraderManager Get()
	{
		if (!s_Instance)
			s_Instance = new EoH_RT_TraderManager();
		return s_Instance;
	}

	void EoH_RT_TraderManager()
	{
		m_Runtimes = new map<string, ref EoH_RT_TraderRuntime>();
		m_Revealed = new map<string, bool>();
		m_Config = EoH_RT_GlobalConfig.Get();
		Init();
	}

	void Init()
	{
		foreach (EoH_RT_TraderProfile profile : m_Config.Traders)
		{
			EoH_RT_TraderRuntime rt = new EoH_RT_TraderRuntime();
			rt.TraderId = profile.TraderId;
			m_Runtimes.Set(profile.TraderId, rt);
			m_Revealed.Set(profile.TraderId, false);
		}
	}

	void Reveal(string traderId, vector pos, string label)
	{
		if (m_Revealed.Get(traderId))
			return;

		m_Revealed.Set(traderId, true);

		EoH_MarkerData data = new EoH_MarkerData(traderId, label, pos);
		data.Category = EoH_MarkerCategory.TRADER;
		data.Pulse = 1;

		EoH_MarkerService.Broadcast(data);
	}

	void Clear(string traderId)
	{
		m_Revealed.Set(traderId, false);
		EoH_MarkerService.RemoveFromAll(traderId);
	}
};
