# Altar Relay Towers Repair Lifecycle

This document records the approved Altar Relay Towers weekend-event design for Echoes of Humanity.

## Core behavior

- Altar communications remain DOWN until the relay is fully repaired.
- Intel reveals the Altar objective and keeps the event marker active.
- The event itself does not reset when a repair attempt fails.
- A failed repair only resets the active repair attempt.
- Another team can start a new repair attempt from 0 percent after the previous repair team is wiped or pushed out.

## Repair start

A repair attempt starts when an eligible player is close to the relay start point and the team has the required components.

Recommended config:

```json
{
  "EnableAltarProximityAutoRepair": true,
  "AltarRepairDurationSeconds": 60.0,
  "AltarRepairProximityRadius": 5.0,
  "AltarRepairMaintainRadius": 75.0,
  "AltarRepairWatcherIntervalMs": 1000,
  "AltarAIRounds": 5,
  "AltarRewardWave": 5
}
```

Required repair components:

- Field Transceiver or approved radio equivalent.
- Car Battery.

Components should be consumed only when repair reaches 100 percent, not when the repair starts.

## Group maintenance rule

When a repair attempt starts, the repair is locked to the starting player's group/team.

- The starter does not have to stay in the 75m zone.
- At least one alive member of the active repair group must remain within 75m.
- If all alive members of the active repair group leave the 75m zone or are killed, the repair attempt fails.
- Repair progress resets to 0.
- Relay remains DOWN.
- Event marker remains active.
- A different group may then start a new repair attempt from 0 percent.

## AI wave behavior

AI begins as soon as repair starts.

- Round 1 spawns immediately at repair start.
- Round 2 spawns later during the repair/event timeline.
- Round 3 spawns later.
- Round 4 spawns later.
- Round 5 spawns as the final defense round.

There are five total AI rounds.

## Reward timing

Reward crate and red smoke spawn when Round 5 is spawned.

They do not spawn:

- On intel reveal.
- On repair start.
- On repair completion.
- On failed repair attempts before Round 5.

Round 5 is the reward defense round.

## Correct high-level flow

```text
Intel reveals Altar
Relay communications DOWN
Repair watcher starts
Team starts repair
Repair progress UI opens
Round 1 AI spawns immediately
At least one alive repair-group member must remain within 75m
Rounds 2-4 escalate over time
Round 5 spawns
Reward crate and red smoke spawn
Repair reaches 100 percent
Repair components are consumed
Relay communications go ONLINE
Reward window continues
Cleanup removes AI, marker, smoke, and leftover crate
```

## Failure and takeover case

```text
Group A starts repair
Group A reaches partial progress
Group B attacks
Group A is killed or fully pushed outside 75m
Group A repair attempt fails
Progress resets to 0
Relay remains DOWN
Group B can start a new repair attempt from 0 if they meet requirements
```

## Notes for implementation

The existing repo places the relevant files under:

```text
EoH_Server/scripts/3_Game/EoH_WeeklyEvents/eoh_weekly_event_config.c
EoH_Server/scripts/4_World/EoH_WeeklyEvents/eoh_event_objective_manager.c
EoH_Server/scripts/4_World/EoH_WeeklyEvents/eoh_event_wave_manager.c
EoH_Server/scripts/4_World/EoH_WeeklyEvents/eoh_event_objective.c
```

The current live objective manager path previously spawned the reward crate and red smoke immediately inside `ActivateObjectiveFromRepair`. That behavior should be replaced so `ActivateObjectiveFromRepair` starts the repair/combat runtime and wave 1, while the reward crate/smoke are triggered only from the wave-5 spawn path.
