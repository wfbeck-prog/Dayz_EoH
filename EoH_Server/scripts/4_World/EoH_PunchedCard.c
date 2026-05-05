modded class PunchedCard : Paper
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionUseUndergroundPanel);
    }
}

modded class ActionUseUndergroundPanel
{
    override void OnFinishProgressServer(ActionData action_data)
    {
        super.OnFinishProgressServer(action_data);

        if (!action_data || !action_data.m_MainItem)
            return;

        PunchedCard card = PunchedCard.Cast(action_data.m_MainItem);
        if (!card)
            return;

        card.AddHealthLevel(4);

        EoH_WorldStateManager.Get().TriggerBunkerOpened();
    }
}
