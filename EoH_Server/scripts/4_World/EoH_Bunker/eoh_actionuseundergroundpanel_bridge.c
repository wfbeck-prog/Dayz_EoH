modded class ActionUseUndergroundPanel
{
    override void OnStartServer(ActionData action_data)
    {
        super.OnStartServer(action_data);

        if (!action_data || !action_data.m_Player)
            return;

        EoH_BunkerGlobalAlert.Send(action_data.m_Player);
    }
}
