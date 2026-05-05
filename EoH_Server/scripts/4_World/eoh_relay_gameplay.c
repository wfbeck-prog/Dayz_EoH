class EoH_RelayGameplay
{
    protected static ref map<string, int> s_LastActivationByTown;

    static void Ensure()
    {
        if (!s_LastActivationByTown)
            s_LastActivationByTown = new map<string, int>();
    }

    static bool CanActivate(string town, PlayerBase player)
    {
        Ensure();

        if (town == "")
            return false;

        int now = GetGame().GetTime();
        int last = 0;
        s_LastActivationByTown.Find(town, last);

        if (last > 0 && now - last < 300000)
        {
            if (player)
                player.MessageStatus("Relay is still recalibrating. Try again soon.");
            return false;
        }

        return true;
    }

    static void ActivateRelay(PlayerBase player, EoH_CaptureRelay_Base relay)
    {
        if (!GetGame().IsServer() || !player || !relay)
            return;

        string town = relay.GetEoHTownName();
        if (town == "")
            return;

        if (!CanActivate(town, player))
            return;

        Ensure();
        s_LastActivationByTown.Set(town, GetGame().GetTime());

        EoH_CaptureManager.Get().StartCapture(town, player);
        BroadcastRelayActivation(player, town, relay.GetPosition());
        SendRelayMarker(town, relay.GetPosition());
        EoH_IntelManager.Get().RevealTownIntel(player);
    }

    static void BroadcastRelayActivation(PlayerBase activator, string town, vector pos)
    {
        string name = "Unknown Survivor";
        if (activator && activator.GetIdentity())
            name = activator.GetIdentity().GetName();

        string msg = "Radio relay activated near " + town + " by " + name + ".";

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.GetIdentity())
                continue;

            player.MessageStatus(msg);
        }
    }

    static void SendRelayMarker(string town, vector pos)
    {
        EoH_MarkerData data = new EoH_MarkerData("EoH_RELAY_ACTIVE_" + town, "Relay Active: " + town, pos);
        data.Category = EoH_MarkerCategory.INTEL;
        data.State = EoH_MarkerState.CONTESTED;
        data.Icon = "Radio";
        data.Is3D = 0;
        data.Pulse = 1;
        data.Color = ARGB(255, 255, 220, 80);
        data.BaseColor = data.Color;
        data.Normalize();

        EoH_MarkerService.Broadcast(data);

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RemoveRelayMarker, 300000, false, town);
    }

    static void RemoveRelayMarker(string town)
    {
        EoH_MarkerService.RemoveFromAll("EoH_RELAY_ACTIVE_" + town);
    }
}
