# Shared Objective UI Plan

This document records the approved plan for shared EoH objective UI elements.

The Altar Relay repair overlay proved that capture-style UI works in-game. The next step is to separate EoH objective UI into two related but distinct systems:

```text
Objective Progress UI
Objective Wave UI
```

## UI categories

### Objective Progress UI

Used when players are capturing, repairing, holding, or controlling an objective.

Systems:

- Altar Relay repair.
- Town Relay Capture.
- KOTH hold/control.
- Future bunker/keyroom/signal objectives that use timers or progress.

Example:

```text
ALTAR RELAY REPAIR
Signal tether active.
[progress bar] 54%
Team: 1     Radius: 75m
```

Example Town Capture:

```text
RELAY CONTROL: NOVY SOBOR
Broadcasting influence signal...
[progress bar] 42%
Group: EXP_3     Contested: No
```

Example KOTH:

```text
KOTH: GREEN MOUNTAIN
Holding signal zone...
[progress bar] 67%
Players: 2     Contested: Yes
```

### Objective Wave UI

Used only when an event has discrete AI wave stages.

Systems:

- Altar Relay hostile response.
- Purge Night AI waves.
- Future weekend events with AI waves.

Not used for:

- Town Capture.
- KOTH.

Town AI is defensive ownership support, not a wave system. KOTH is a control/hold system, not a wave system.

Example Altar Wave UI:

```text
HOSTILE RESPONSE
Wave 3 / 5
Armed response units are converging.
```

Example Purge Night Wave UI:

```text
PURGE NIGHT
Wave 2 / 3
The purge signal is intensifying.
```

## Current Altar Progress UI

Current files:

```text
EoH_Server/scripts/3_Game/EoH_WeeklyEvents/eoh_altar_repair_progress_data.c
EoH_Server/scripts/5_Mission/EoH_WeeklyEvents/eoh_altar_repair_progress_client.c
EoH_Server/scripts/5_Mission/EoH_WeeklyEvents/layouts/eoh_altar_repair_progress.layout
```

Current Altar display:

```text
ALTAR RELAY REPAIR
Signal tether active. Hold the 75m relay zone.
Progress: 0-100%
Team: #
Radius: 75m
```

## Generalized Objective Progress design

Create a generic objective-progress DTO and client overlay.

Suggested files:

```text
EoH_Server/scripts/3_Game/EoH_UI/eoh_objective_progress_data.c
EoH_Server/scripts/5_Mission/EoH_UI/eoh_objective_progress_client.c
EoH_Server/layouts/eoh_objective_progress.layout
```

Suggested DTO:

```c
class EoH_ObjectiveProgressData
{
    string Channel;
    string Title;
    string Status;
    string LeftLabel;
    string RightLabel;

    float Progress01;
    int ProgressPercent;

    bool Complete;
    bool Failed;
    bool Contested;
}
```

## Generalized Objective Wave design

Create a generic wave-status DTO and client overlay.

Suggested files:

```text
EoH_Server/scripts/3_Game/EoH_UI/eoh_objective_wave_data.c
EoH_Server/scripts/5_Mission/EoH_UI/eoh_objective_wave_client.c
EoH_Server/layouts/eoh_objective_wave.layout
```

Suggested DTO:

```c
class EoH_ObjectiveWaveData
{
    string Channel;
    string Title;
    string Status;

    int CurrentWave;
    int MaxWaves;

    bool FinalWave;
    bool RewardDeployed;
    bool Complete;
}
```

## Altar usage

Altar Relay Repair would use Objective Progress UI:

```text
Channel = "altar_relay"
Title = "ALTAR RELAY REPAIR"
Status = "Signal tether active. Hold the 75m relay zone."
LeftLabel = "Team: 1"
RightLabel = "Radius: 75m"
ProgressPercent = 54
```

Altar hostile response would use Objective Wave UI:

```text
Channel = "altar_relay"
Title = "HOSTILE RESPONSE"
Status = "Armed response units are converging."
CurrentWave = 3
MaxWaves = 5
FinalWave = false
RewardDeployed = false
```

On Wave 5:

```text
Channel = "altar_relay"
Title = "FINAL HOSTILE RESPONSE"
Status = "Recovery cache deployed. Red smoke active."
CurrentWave = 5
MaxWaves = 5
FinalWave = true
RewardDeployed = true
```

## Purge Night usage

Purge Night should use Objective Wave UI:

```text
Channel = "purge_night"
Title = "PURGE NIGHT"
Status = "The purge signal is intensifying."
CurrentWave = 2
MaxWaves = 3
FinalWave = false
RewardDeployed = false
```

On final Purge wave/reward phase:

```text
Channel = "purge_night"
Title = "PURGE NIGHT"
Status = "Final purge window. Recovery cache exposure is imminent."
CurrentWave = 3
MaxWaves = 3
FinalWave = true
RewardDeployed = false
```

## Town Capture usage

Town Relay Capture should use Objective Progress UI only:

```text
Channel = "town_capture"
Title = "RELAY CONTROL: NOVY SOBOR"
Status = "Broadcasting influence signal..."
LeftLabel = "Group: EXP_3"
RightLabel = "Owner: None"
ProgressPercent = 42
```

If contested:

```text
Title = "RELAY CONTROL: NOVY SOBOR"
Status = "Signal contested by hostile group. Capture paused."
LeftLabel = "Group: EXP_3"
RightLabel = "Hostiles: 1"
Contested = true
```

Town AI should eventually show defensive support status, not wave count:

```text
TOWN DEFENSE
Owner: EXP_3
Defenders active: 4
Threat detected: Yes
```

## KOTH usage

KOTH should use Objective Progress UI only:

```text
Channel = "koth"
Title = "KOTH: GREEN MOUNTAIN"
Status = "Holding signal zone..."
LeftLabel = "Players: 2"
RightLabel = "Contested: Yes"
ProgressPercent = 67
```

## Migration strategy

1. Keep the current Altar-specific progress UI working.
2. Add the generic shared progress overlay in parallel.
3. Add the generic shared wave overlay in parallel.
4. Wire Town Relay Capture to the shared progress overlay.
5. Wire KOTH to the shared progress overlay.
6. Wire Altar waves to the shared wave overlay.
7. Wire Purge Night waves to the shared wave overlay.
8. After shared UI proves stable, migrate Altar repair from the Altar-specific overlay to the shared progress overlay.
9. Remove the Altar-specific duplicate only after all systems are stable.

## Design rules

- Do not duplicate layouts for each event.
- Keep progress overlays visually consistent.
- Keep wave overlays visually consistent.
- Keep all UI text fully data-driven from the server.
- Use separate `Channel` values so different systems can show/hide/update independently if needed.
- Avoid hard-coding Altar, Town Capture, KOTH, or Purge wording in the generic clients.

## Priority

Current priority order:

1. Finish Altar gameplay validation and balance.
2. Make Altar wave timing configurable.
3. Build shared Objective Progress UI.
4. Connect Town Relay Capture to shared progress UI.
5. Connect KOTH to shared progress UI.
6. Build shared Objective Wave UI.
7. Connect Altar and Purge Night to shared wave UI.
