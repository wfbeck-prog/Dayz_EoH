# EoH Quest Generation Status

## Current State

The old quest and objective JSON folders have been cleaned.

This pack is now ready for clean generation from the EoH quest manifest.

## Source of Truth

```text
Server_Config_Backup/HostHavocDayZServer/ExpansionMod/Quests/EoH_Quest_Manifest.json
```

## Output Structure

Quest JSON files:

```text
Server_Config_Backup/HostHavocDayZServer/ExpansionMod/Quests/Quests/
```

Objective JSON files:

```text
Server_Config_Backup/HostHavocDayZServer/ExpansionMod/Quests/Objectives/<ObjectiveType>/
```

## Allowed Objective Types

- AICamp
- AIPatrol
- Action
- Collection
- Crafting
- Delivery
- Target
- Travel
- TreasureHunt

## Avoided Objective Types

- AIVIP

## Confirmed Rules

- NPC 3 handles standard / early / mid progression.
- NPC 1 handles elite boss-chain progression.
- Punch cards must never be direct quest rewards.
- Punch cards only come from Tier 4 CBD loot room drops.
- Final three elite quests target:
  - 400198: SKSuperman
  - 400199: All Out Genius
  - 400200: DeadlyDead SK

## Generation Plan

### Slice 1

```text
400001-400030
Arc 1 — EoH Initiation
NPC 3
Objective folders: Travel, Collection, Delivery, Action
```

This slice is the validation baseline before generating the remaining 170 quests.

## Validation Checklist

- Quest JSONs load under `Quests/Quests/`.
- Objective JSONs load under correct typed folders.
- Every quest objective ID has a matching objective file.
- `PreQuestIDs` and `FollowUpQuest` create intentional chains.
- First quest in Arc 1 has no prerequisite.
- Last quest in Arc 1 points to Arc 2 start or ends cleanly until Arc 2 is generated.
- Rewards are meaningful but do not bypass CBD/bunker progression.
