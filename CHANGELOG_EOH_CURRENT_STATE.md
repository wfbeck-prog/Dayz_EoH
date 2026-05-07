# EoH Current State Changelog

## Current Focus

Live gameplay systems stabilization.

Quest system v2 generation paused temporarily while bunker/CBD feedback systems are debugged.

Current bunker direction is now locked: bunker loot should be true endgame combat reward loot, while traders remain useful for limited ammo and magazine support.

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
- CJ187-LootChest bunker direction chosen.
- MMG Base Storage deployed containers will be used as the actual CJ187 bunker loot containers.
- Bunker loot will exclude filler categories:
  - no sidearms
  - no general supplies
  - no tools
  - no medical loot

## Bunker Loot Direction

Bunker loot should focus only on endgame combat and raid progression:

- Epic weapons.
- High-power sniper rifles.
- Epic gear.
- Ammo.
- Magazines.
- Attachments and optics.
- Breaching Charge mod crafting components.

Confirmed bunker-tier weapon direction:

```text
SNAFUAlligator
GCGN_M82_Barret
GCGN_M700
SNHK417_Gun
SCAR_Gun
SNSA58_Gun
SNAFU_RPK16_Black
SNAFU_RPK16_Green
SNAFU_RPK16_Tan
GCGN_M249
HK416_Gun
GCGN_HK416
GCGN_M4A1
MK18_Gun
ACR_Gun
```

Confirmed/targeted high-value mag, ammo, and optic support:

```text
GCGN_M82_Mag
GCGN_M700_Mag
SNHK417_Mag
GCGN_Mag_FAL_50Rnd
SA58_DrumMag
GCGN_M249_Mag
GCGN_HK416_Mag
GCGN_Ammo_50Cal
GCGN_Ammo_408Chey
GCGN_Ammo_3006
SNAFU_Nightforce
SNAFU_Leupold_Mark8
SNAFU_Kahles
```

## Bunker Container Layout

Seven hand-placed bunker containers are planned through CJ187-LootChest using MMG deployed container classes.

Recommended roles:

1. Epic weapon crate.
2. Heavy sniper case.
3. Ammunition cache.
4. Epic gear locker.
5. Breaching safe.
6. Explosives cabinet.
7. Final cache.

CJ187 bunker container rules:

```text
respawn: 0
randomize_position: 0
```

Reason:
- Prevent bunker farm loops.
- Preserve exact hand-placed bunker layout.
- Make each bunker opening feel like a server event.

## Breaching Charge Loot Direction

Bunker loot should include Breaching Charge mod crafting parts and rare completed charge items.

Confirmed breaching-related classnames:

```text
HDSN_Charcoal
HDSN_Sulfur
HDSN_Gunpowder
HDSN_Explosive
HDSN_BreachingCharge
HDSN_C4Stick
HDSN_C4Sticks
HDSN_Detonator
HDSN_C4NoWiring
HDSN_BreachingChargeHeavy
```

Balance target:
- Components should be more common than completed breaching charges.
- Completed breaching charges should be rare.
- Heavy breaching charges should be very rare.

## Trader Economy Rules

All weapons that spawn in the bunker will be removed from trader sell inventories.

Ammo and magazines will remain available at traders, but in limited quantities.

Recommended trader split:

- Normal traders:
  - no bunker weapons
  - limited common ammo
  - limited common mags
  - no .50 Cal / .408 Chey as normal stock
- Black market:
  - may buy bunker weapons
  - does not sell bunker weapons
  - may sell very limited rare ammo/mags
  - may sell very limited elite optics if needed as a money sink
- Bunker:
  - only reliable source of epic weapons
  - best source of elite optics
  - best source of .50 Cal / .408 Chey ammo
  - best source of breaching parts

Trader removal list:

```text
SNAFUAlligator
GCGN_M82_Barret
GCGN_M700
SNHK417_Gun
SCAR_Gun
SNSA58_Gun
SNAFU_RPK16_Black
SNAFU_RPK16_Green
SNAFU_RPK16_Tan
GCGN_M249
HK416_Gun
GCGN_HK416
GCGN_M4A1
MK18_Gun
ACR_Gun
```

Limited trader/black-market ammo and mag candidates:

```text
GCGN_M82_Mag
GCGN_M700_Mag
SNHK417_Mag
GCGN_Mag_FAL_50Rnd
SA58_DrumMag
GCGN_M249_Mag
GCGN_HK416_Mag
GCGN_Ammo_50Cal
GCGN_Ammo_408Chey
GCGN_Ammo_3006
```

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

## Bunker Loot Config Not Yet Finalized

Need to generate/merge final CJ187 JSON tables for:
- MMG bunker container locations.
- Epic weapon loot tables.
- High-power sniper tables.
- Epic gear tables.
- Ammo/mag/attachment tables.
- Breaching charge component tables.

## Trader Config Not Yet Updated

Expansion Market trader files still need to be located or uploaded before removing bunker weapons from actual trader sell inventories.

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
3. Generate and merge final CJ187 bunker loot config.
4. Remove bunker weapons from trader sell inventories.
5. Limit bunker ammo and magazines at traders/black market.
6. Implement CBD room-open marker flash system.
7. Validate relay/town gameplay loop.
8. Resume high-quality handcrafted quest generation.
