# Shared Objective Progress UI

This document records the approved plan to reuse the new Altar Relay repair overlay for Town Relay Capture and future objective systems.

## Goal

The current Altar Relay repair UI proves that a capture-style progress overlay works in-game. The next step is to generalize it so multiple EoH systems can use the same visual language.

Target systems:

- Altar Relay repair.
- Town Relay Capture.
- Future weekend events.
- Possible bunker/keyroom/signal objectives.

## Current Altar UI

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
Repair team in zone: #
Maintain radius: 75m
```

## Generalized design

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

## Altar usage

Altar Relay Repair would send:

```text
Channel = "altar_relay"
Title = "ALTAR RELAY REPAIR"
Status = "Signal tether active. Hold the 75m relay zone."
LeftLabel = "Repair team in zone: 1"
RightLabel = "Maintain radius: 75m"
ProgressPercent = 54
```

## Town Capture usage

Town Relay Capture should send:

```text
Channel = "town_capture"
Title = "RELAY CONTROL: NOVY SOBOR"
Status = "Broadcasting influence signal..."
LeftLabel = "Group in zone: 3"
RightLabel = "Town owner: EXP_3"
ProgressPercent = 42
```

If contested:

```text
Title = "RELAY CONTROL: NOVY SOBOR"
Status = "Signal contested by hostile group. Capture paused."
LeftLabel = "Group in zone: 2"
RightLabel = "Hostiles in zone: 1"
Contested = true
```

## Migration strategy

1. Keep the current Altar-specific UI working.
2. Add the generic shared overlay in parallel.
3. Wire Town Relay Capture to the shared overlay first.
4. After Town Capture proves stable, migrate Altar from the Altar-specific overlay to the shared overlay.
5. Remove the Altar-specific duplicate only after both systems are stable.

## Town Capture integration notes

The town capture system already uses EoH group ownership and relay logic. The shared UI should expose:

- Current town name.
- Capturing group name or ID.
- Current owner.
- Capture progress.
- Group members inside relay zone.
- Hostile/contesting group count.
- Paused/contested state.

## Design rules

- Do not duplicate layouts for each event.
- Keep all progress overlays visually consistent.
- Keep the UI text fully data-driven from the server.
- Use separate `Channel` values so different systems can show/hide/update independently if needed.
- Avoid hard-coding Altar wording in the generic client.

## Priority

Finish and validate Altar AI spawning first. Then build the shared objective progress layer and connect it to Town Relay Capture.
