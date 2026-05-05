modded class PunchedCard : Paper
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionUseUndergroundPanel);
    }

    override void OnActionCompleted(ActionData action_data)
    {
        super.OnActionCompleted(action_data);

        if (!action_data || !action_data.m_MainItem)
            return;

        if (action_data.m_Action && action_data.m_Action.IsKindOf(ActionUseUndergroundPanel))
        {
            AddHealthLevel(4);
        }
    }
}
