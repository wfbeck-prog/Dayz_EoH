class EoH_RT_TraderNPC extends SurvivorBase
{
	protected string m_TraderId;
	protected string m_ExpansionTraderUID;
	protected bool m_IsBusy;

	void EoH_RT_TraderNPC()
	{
		m_TraderId = "";
		m_ExpansionTraderUID = "";
		m_IsBusy = false;
	}

	void SetTraderIdentity(string traderId, string expansionTraderUID)
	{
		m_TraderId = traderId;
		m_ExpansionTraderUID = expansionTraderUID;
	}

	string GetTraderId()
	{
		return m_TraderId;
	}

	string GetExpansionTraderUID()
	{
		return m_ExpansionTraderUID;
	}

	bool CanInteract(PlayerBase player)
	{
		if (!player)
			return false;
		return vector.Distance(player.GetPosition(), GetPosition()) <= 3.0;
	}
};
