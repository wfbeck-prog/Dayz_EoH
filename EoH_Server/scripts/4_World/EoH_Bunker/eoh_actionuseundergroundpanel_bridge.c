modded class ActionUseUndergroundPanel
{
    override void OnStartServer(ActionData action_data)
    {
        super.OnStartServer(action_data);

        if (!action_data || !action_data.m_Player)
            return;

        EoH_RuinPunchCard(action_data.m_MainItem);
        EoH_BunkerGlobalAlert.Send(action_data.m_Player);
    }

    void EoH_RuinPunchCard(ItemBase item)
    {
        if (!item)
            return;

        PunchedCard card = PunchedCard.Cast(item);
        if (!card)
            return;

        card.SetHealth("", "Health", 0);
        card.SetHealth01("", "Health", 0.0);
        card.SetSynchDirty();
    }
}
