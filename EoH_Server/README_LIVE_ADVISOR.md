# EoH Live Advisor

The EoH Live Advisor is a server-side reporting layer built into the `EoH_Server` mod.

It is designed for HostHavoc-style hosting where only the web console and file manager may be available.

## Current Mode

Advisor-only mode.

The system does not restart the server, edit live configs, punish players, spawn loot, change the economy, or call external services from inside DayZ.

It writes advisor-ready logs that can be copied from the server profile folder and pasted into ChatGPT for analysis.

## Files Packed in the PBO

Include:

```text
EoH_Server/config.cpp
EoH_Server/scripts/3_Game/EoH_LiveAdvisor/
EoH_Server/scripts/4_World/EoH_LiveAdvisor/
EoH_Server/scripts/5_Mission/EoH_LiveAdvisor/
```

Do not include:

```text
EoH_Server/tools/
```

The `tools` folder is not required for the HostHavoc-friendly advisor-only build.

## Generated Runtime Files

After the server starts, the mod creates:

```text
profiles/EoH/LiveAdvisor/LiveAdvisorSettings.json
profiles/EoH/LiveAdvisor/events.log
profiles/EoH/LiveAdvisor/latest_report.txt
```

## Expected Startup Output

`latest_report.txt` should contain entries similar to:

```text
=== Echoes of Humanity Live Advisor Report ===
Server: Echoes of Humanity Hardcore
Mode: Advisor Only
ConfigVersion: 2
MaxReportLines: 75

[info] [EoH_Server] [SERVER_START] EoH Live Advisor initialized
[info] [EoH_Server] [SELF_TEST] Live Advisor file logging and report generation are online
[info] [MissionServer] [MISSION_INIT] MissionServer initialized Live Advisor
```

`events.log` should also show heartbeat entries after the configured interval:

```text
[info] [MissionServer] [SERVER_HEARTBEAT] Server heartbeat. Players online: 0
```

## Configuration

The generated file is:

```text
profiles/EoH/LiveAdvisor/LiveAdvisorSettings.json
```

Important fields:

```json
{
  "ConfigVersion": 2,
  "Enabled": true,
  "AdvisorOnly": true,
  "ServerName": "Echoes of Humanity Hardcore",
  "HeartbeatSeconds": 300,
  "LogPlayerCounts": true,
  "LogDebugToRPT": true,
  "WriteCopyPasteReport": true,
  "MaxReportLines": 75
}
```

For testing, set `HeartbeatSeconds` to `60`.

## Test Status

Validated behavior:

- Server start event writes successfully.
- Self-test event writes successfully.
- MissionServer init event writes successfully.
- Heartbeat event writes successfully.
- Player count changes are logged.
- `latest_report.txt` is generated for copy/paste analysis.

## Next Planned Hooks

Recommended next additions:

1. `BUNKER_PANEL_MISSING`
2. `PUNCHCARD_USED`
3. `BUNKER_OPENED`
4. Roaming trader movement/discovery events
5. Expansion quest diagnostics
6. AI spawn/loadout warnings
