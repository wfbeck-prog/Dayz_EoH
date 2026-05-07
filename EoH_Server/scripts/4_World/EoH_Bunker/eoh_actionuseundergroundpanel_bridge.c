modded class ActionUseUndergroundPanel
{
    override void OnStartServer(ActionData action_data)
    {
        Print("[EoH_Bunker] ActionUseUndergroundPanel bridge OnStartServer fired");

        super.OnStartServer(action_data);

        if (!action_data)
        {
            Print("[EoH_Bunker] ActionUseUndergroundPanel bridge failed: action_data is null");
            return;
        }

        if (!action_data.m_Player)
        {
            Print("[EoH_Bunker] ActionUseUndergroundPanel bridge failed: player is null");
            return;
        }

        if (action_data.m_Player.GetIdentity())
            Print("[EoH_Bunker] Underground panel used by " + action_data.m_Player.GetIdentity().GetName());
        else
            Print("[EoH_Bunker] Underground panel used by player with no identity");

        EoH_RuinPunchCard(action_data.m_MainItem);
        EoH_BunkerGlobalAlert.Send(action_data.m_Player);
    }

    void EoH_RuinPunchCard(ItemBase item)
    {
        if (!item)
        {
            Print("[EoH_Bunker] Punch card ruin skipped: main item is null");
            return;
        }

        PunchedCard card = PunchedCard.Cast(item);
        if (!card)
        {
            Print("[EoH_Bunker] Punch card ruin skipped: main item is not PunchedCard, type=" + item.GetType());
            return;
        }

        card.SetHealth("", "Health", 0);
        card.SetHealth01("", "Health", 0.0);
        card.SetSynchDirty();

        Print("[EoH_Bunker] Punch card ruined after bunker panel use");
    }
}
