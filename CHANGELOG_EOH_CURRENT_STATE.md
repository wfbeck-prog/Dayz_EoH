# EoH Current State Changelog

## Current Focus

Live gameplay systems stabilization.

Quest system v2 generation paused temporarily while bunker/CBD feedback systems are debugged.

---

# Completed / Working Systems

## Relay Gameplay System

- Relay objects spawning.
- Relay ownership/capture groundwork.
- Group-name integration.
- Radio-based respawn integration.
- Town respawn groundwork.
- Terje StartScreen integration groundwork.

## Terje Respawn Integration

- Flag ownership respawn restriction.
- Town radio respawn integration.
- XML timeout support confirmed.

## Intel System

- Intel books/items created.
- Intel groundwork tied toward future Expansion Book integration.
- Book manager framework started.

## AI / Combat Systems

- Town AI progression.
- AI loot integration.
- SNAFU + MDF loadout direction.
- Tier 3.5 boss AI templates created:
  - SKSuperman
  - All Out Genius
  - DeadlyDead SK

## CBD / Key Rooms

- Converted toward CBD loot-room direction.
- Tiered room progression preserved.
- Punch cards restricted to Tier 4 loot rooms only.
- Key drop integration started.

## Bunker Systems

- Punch card ruin-on-use system added.
- Bunker global alert framework added.
- ActionUseUndergroundPanel bridge added.
- Debug logging added to bunker bridge.

---

# Current Known Issues

## Bunker Notification Not Firing

Debug logging now added to:

```text
EoH_Server/scripts/4_World/EoH_Bunker/eoh_actionuseundergroundpanel_bridge.c
```

Need to confirm via server logs whether:
- ActionUseUndergroundPanel bridge fires.
- Player identity exists.
- Global alert helper executes.

If bridge fails entirely:
- fallback plan is panel-state observer.

## CBD Marker Flash / Global Alerts

Not implemented yet.

Planned:
- Hook after successful CBD UnlockLootRoom execution.
- Send marker pulse/flash.
- Send global room-open notifications.
- Add debug logging first.

---

# Quest System Status

## Old State

- Flat quest pile.
- Broken chaining.
- Poor reward scaling.
- Mixed objective placement.

## New Architecture

- Manifest-driven rebuild.
- 200 planned EoH quests.
- Objective-type folder support.
- No AIVIP usage.
- Proper quest arcs.
- Elite NPC 1 boss chain.
- No direct punch card rewards.

## Current Generated Files

Production examples generated:

```text
Q_400001.json
Q_400002.json
Q_400003.json
Q_400004.json
```

Objective examples generated:

```text
O_500001.json
O_500002.json
O_500003.json
O_500004.json
```

## Quest Generation Issue

QuestPack v1 scaffold output was too repetitive.

Future generation needs:
- unique arc identity
- varied locations
- varied rewards
- varied gameplay loops
- handcrafted EoH narrative quality

Planned future workflow:
- generate ZIP artifact first
- commit ZIP to repo for traceability
- then expand validated JSON files

---

# Recommended Next Session Priorities

1. Verify bunker bridge debug logs.
2. Fix bunker global notifications.
3. Implement CBD room-open marker flash system.
4. Validate relay/town gameplay loop.
5. Resume high-quality handcrafted quest generation.
