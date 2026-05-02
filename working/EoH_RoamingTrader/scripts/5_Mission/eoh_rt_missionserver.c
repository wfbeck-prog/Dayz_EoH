modded class MissionServer
{
	protected ref EoH_RT_TraderManager m_EoH_RT_TraderManager;
	protected ref EoH_CBD_Observer m_EoH_CBD_Observer;

	override void OnInit()
	{
		super.OnInit();

		m_EoH_RT_TraderManager = EoH_RT_TraderManager.GetInstance();
		m_EoH_RT_TraderManager.Initialize();

		m_EoH_CBD_Observer = EoH_CBD_Observer.GetInstance();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_RT_TraderTick, 1000, true);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_CBD_Tick, 1000, true);
	}

	void EoH_RT_TraderTick()
	{
		if (m_EoH_RT_TraderManager)
			m_EoH_RT_TraderManager.Update();
	}

	void EoH_CBD_Tick()
	{
		if (m_EoH_CBD_Observer)
			m_EoH_CBD_Observer.Update();
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

		if (m_EoH_CBD_Observer && player)
			m_EoH_CBD_Observer.SendCurrentMarkersToPlayer(player);
	}
};