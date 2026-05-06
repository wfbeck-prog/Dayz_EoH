# EoH Expansion Quest Rebuild Plan

The current quest pack should not be treated as a finished progression system yet. It was patched into chains, but the better long-term fix is to rebuild it around intentional quest arcs, shared IDs, clean objective ownership, NPC roles, and gated progression.

## Problem

The quests are currently behaving like a flat list because many quest files were generated with:

```json
"FollowUpQuest": -1,
"PreQuestIDs": []
```

That makes Expansion treat them as standalone quests instead of narrative/progression chains.

## Target Design

Rebuild quests into deliberate EoH arcs instead of patching IDs one by one.

### NPC Roles

- NPC 1: High-tier / black market / bunker / endgame progression
- NPC 3: Early-to-mid EoH survival, intel, town, relay, trader discovery

## Proposed Quest Arcs

### Arc 1 — EoH Orientation

Purpose: introduce lore, survival basics, and the Echoes of Humanity identity.

Suggested ID range:

```text
400001 - 400020
```

NPC:

```text
3
```

### Arc 2 — Signal / Intel Discovery

Purpose: introduce intel items, signal traces, roaming trader clues, and world investigation.

Suggested ID range:

```text
400021 - 400060
```

NPC:

```text
3
```

### Arc 3 — Town Relay Control

Purpose: teach players to find relays, activate relays, contest towns, and defend controlled infrastructure.

Suggested ID range:

```text
400061 - 400100
```

NPC:

```text
3
```

### Arc 4 — Drug Economy / Black Market Contact

Purpose: move players into high-risk economy routes, drug trader discovery, black market progression, and AI threats.

Suggested ID range:

```text
400101 - 400150
```

NPC:

```text
3 early handoff, NPC 1 for black market handoff
```

### Arc 5 — High-Tier Black Market / Bunker

Purpose: late-game bunker, keycard, high-value loot, epic-tier access, and dangerous objectives.

Suggested ID range:

```text
400151 - 400220
```

NPC:

```text
1
```

## Chaining Rules

Each quest should have one of the following relationship styles.

### Linear Quest

```json
"FollowUpQuest": 400002,
"PreQuestIDs": []
```

Next quest:

```json
"FollowUpQuest": 400003,
"PreQuestIDs": [400001]
```

### Arc Start

```json
"PreQuestIDs": [previous_arc_final_id]
```

### Arc Final

```json
"FollowUpQuest": -1
```

### Branch Unlock

Use multiple quests with the same prerequisite:

```json
"PreQuestIDs": [400060]
```

This should only be used intentionally, not accidentally.

## Objective Rules

Each quest must be checked against its objective files.

For every quest:

```json
"Objectives": [
  {
    "ID": objective_id,
    "ObjectiveType": objective_type
  }
]
```

There must be a matching objective config in the correct objective folder.

## Rebuild Checklist

1. Inventory all quest files.
2. Inventory all objective files.
3. Build a quest manifest CSV/JSON with:
   - Quest ID
   - Title
   - NPC giver
   - NPC turn-in
   - Objective IDs
   - Objective types
   - Intended arc
   - Previous quest
   - Follow-up quest
4. Validate every objective reference exists.
5. Validate every quest ID is unique.
6. Validate every objective ID is unique or intentionally reused.
7. Assign NPC 1 to high-tier quests only.
8. Assign NPC 3 to early/mid quests.
9. Rewrite chain fields.
10. Re-export clean quest pack.
11. Test in-game with one character from fresh state.

## Important Decision

Do not keep patching individual quest files until the manifest exists. The manifest should become the source of truth, and quest JSON should be generated or validated from it.
