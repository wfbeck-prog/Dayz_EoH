modded class ActionUseUndergroundPanel
{
    override void OnFinishProgressServer(ActionData action_data)
    {
        super.OnFinishProgressServer(action_data);

        if (!action_data || !action_data.m_Player)
            return;

        EoH_BunkerGlobalAlert.Send(action_data.m_Player);
    }
}
