class EoH_EventWaveManager
{
    protected static ref EoH_EventWaveManager s_Instance;

    static EoH_EventWaveManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_EventWaveManager();

        return s_Instance;
    }

    void SpawnWave(EoH_EventObjectiveRuntime runtime, int waveIndex)
    {
        if (!runtime || !runtime.Config)
            return;

        EoH_EventObjective cfg = runtime.Config;
        string objectiveName = cfg.DisplayName;
        string msg = "";

        Print("[EoH_EventWaves] Spawning wave=" + waveIndex.ToString() + " objective=" + cfg.Id);

        // Placeholder for future AI integrations.
        // This intentionally avoids heavy AI spawning loops until live testing is complete.

        if (cfg.Id == "altar_relay_towers")
        {
            SendAltarWaveNotification(objectiveName, waveIndex);
            return;
        }

        if (waveIndex == 1)
        {
            msg = "Scouting activity detected near " + objectiveName + ".";
            EoH_Notifications.SendToAll("EVENT ALERT", msg);
        }
        else if (waveIndex == 2)
        {
            msg = "Hostile reinforcements moving toward " + objectiveName + ".";
            EoH_Notifications.SendToAll("EVENT ALERT", msg);
        }
        else if (waveIndex >= 3)
        {
            msg = "Elite hostile presence confirmed near " + objectiveName + ".";
            EoH_Notifications.SendToAll("EVENT ALERT", msg);
        }
    }

    protected void SendAltarWaveNotification(string objectiveName, int waveIndex)
    {
        string msg = "";

        if (waveIndex == 1)
            msg = "Round 1: Scouting activity detected near " + objectiveName + ".";
        else if (waveIndex == 2)
            msg = "Round 2: Hostile reinforcements are moving toward the relay signal.";
        else if (waveIndex == 3)
            msg = "Round 3: Armed response units are converging on " + objectiveName + ".";
        else if (waveIndex == 4)
            msg = "Round 4: Heavy hostile contact confirmed. Hold the relay zone.";
        else if (waveIndex >= 5)
            msg = "Round 5: Final hostile response detected. Recovery cache deployment authorized.";

        if (msg != "")
            EoH_Notifications.SendToAll("ALTAR RELAY", msg);
    }
}
