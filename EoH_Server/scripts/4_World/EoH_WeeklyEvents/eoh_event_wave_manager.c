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
        {
            EoH_LiveAdvisorActivity.LogActivity("weekly_event", "wave_request_failed reason=missing_runtime wave=" + waveIndex.ToString());
            return;
        }

        EoH_EventObjective cfg = runtime.Config;
        string objectiveName = cfg.DisplayName;
        string msg = "";

        Print("[EoH_EventWaves] Spawning wave=" + waveIndex.ToString() + " objective=" + cfg.Id);
        EoH_LiveAdvisorActivity.LogActivity("weekly_event", "wave_requested objective=" + cfg.Id + " wave=" + waveIndex.ToString() + " type=" + cfg.ObjectiveType + " radius=" + cfg.Radius.ToString());

        if (cfg.Id == "altar_relay_towers")
        {
            SendAltarWaveNotification(objectiveName, waveIndex);
            SpawnAltarWaveAI(runtime, waveIndex);
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

    protected void SpawnAltarWaveAI(EoH_EventObjectiveRuntime runtime, int waveIndex)
    {
        if (!runtime || !runtime.Config)
        {
            EoH_LiveAdvisorActivity.LogActivity("weekly_event", "altar_ai_spawn_failed reason=missing_runtime wave=" + waveIndex.ToString());
            return;
        }

        EoH_EventObjective cfg = runtime.Config;

        int count = 3;
        string loadout = "EoH_AI_Patrol_Assault_DF";

        if (waveIndex == 2)
        {
            count = 4;
            loadout = "EoH_AI_Patrol_Assault_DF";
        }
        else if (waveIndex == 3)
        {
            count = 5;
            loadout = "EoH_AI_Patrol_Marksman_DF";
        }
        else if (waveIndex == 4)
        {
            count = 6;
            loadout = "EoH_AI_HighValue_Hard";
        }
        else if (waveIndex >= 5)
        {
            count = 8;
            loadout = "EoH_AI_HighValue_Hard";
        }

        EoH_LiveAdvisorActivity.LogActivity("weekly_event", "altar_ai_spawn_start wave=" + waveIndex.ToString() + " requested=" + count.ToString() + " loadout=" + loadout + " center=" + cfg.Position.ToString() + " radius=" + cfg.Radius.ToString());

        eAIGroup group = EoH_TownAISpawnAdapter.CreateTownPatrolGroup(cfg.Position);
        if (!group)
            EoH_LiveAdvisorActivity.LogActivity("weekly_event", "altar_ai_group_missing wave=" + waveIndex.ToString() + " loadout=" + loadout);

        int spawned = 0;
        int failed = 0;

        for (int i = 0; i < count; i++)
        {
            vector spawnPos = GetAltarWaveSpawnPosition(cfg.Position, 90.0, Math.Min(280.0, cfg.Radius));
            Object obj = EoH_TownAISpawnAdapter.SpawnTownPatrolUnit("Altar Relay", spawnPos, cfg.Position, loadout, group);

            if (obj)
            {
                spawned++;
            }
            else
            {
                failed++;
                Print("[EoH_AltarRelay][AI][WARN] Failed unit spawn wave=" + waveIndex.ToString() + " loadout=" + loadout + " pos=" + spawnPos.ToString());
            }
        }

        Print("[EoH_AltarRelay][AI] Wave complete wave=" + waveIndex.ToString() + " requested=" + count.ToString() + " spawned=" + spawned.ToString() + " failed=" + failed.ToString() + " loadout=" + loadout);
        EoH_LiveAdvisorActivity.LogActivity("weekly_event", "altar_ai_wave_complete wave=" + waveIndex.ToString() + " requested=" + count.ToString() + " spawned=" + spawned.ToString() + " failed=" + failed.ToString() + " loadout=" + loadout);

        if (spawned <= 0)
        {
            EoH_LiveAdvisorLogger.Log("ADVISOR_RECOMMENDATION", "Category=weekly_event Altar Relay wave requested but no AI units spawned. Wave=" + waveIndex.ToString() + " Requested=" + count.ToString() + " Loadout=" + loadout + ". Check Expansion AI availability, loadout names, spawn adapter, and surface spawn positions.", "warning", "EoH_Advisor");
        }
    }

    protected vector GetAltarWaveSpawnPosition(vector center, float minRadius, float maxRadius)
    {
        float angle = Math.RandomFloat(0, 6.28318);
        float dist = Math.RandomFloat(minRadius, maxRadius);

        vector pos = center;
        pos[0] = center[0] + Math.Cos(angle) * dist;
        pos[2] = center[2] + Math.Sin(angle) * dist;
        pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.1;

        return pos;
    }
}
