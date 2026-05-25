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
}
