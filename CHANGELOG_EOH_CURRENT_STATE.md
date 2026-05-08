# EoH Current State Changelog

## Current Focus

Live gameplay systems stabilization.

Quest system v2 generation paused temporarily while bunker/CBD feedback systems are debugged.

Current bunker direction is locked: bunker loot is true endgame combat reward loot, traders remain useful for limited ammo/magazine support, and raid explosive progression requires repeated bunker runs.

---

# Latest Validation Notes

## AI Bandits Recovery Validation

Confirmed live:

- Safe recovery DynamicAIB schema spawns AI successfully.
- Initial Green Mountain test patrol spawned and killed the player, proving AI_Bandits/AIB_Unleashed itself is working.
- Green Mountain test patrol was removed because Green Mountain is a safe-zone trader area.
- Bunker AI spawned successfully on both bunker levels using the safe recovery config.

Working recovery file:

```text
EoH_Server/config/AIB/DynamicAIB_EoH_SAFE_RECOVERY.json
```

Confirmed bunker groups:

```text
EoH_BUNKER_Upper_BanditGuard_SAFE
EoH_BUNKER_Lower_BanditGuard_SAFE
```

Conclusion:
- AI failure was not caused by AI_Bandits/AIB_Unleashed load order.
- Previous DynamicAIB issue is most likely invalid classnames, invalid attachment chains, risky custom loot, or unsupported schema fields.
- Rebuild AI config in phases using verified classnames only.

Next AI rebuild order:

1. Restore roaming trader escorts with vanilla-safe loadouts.
2. Restore bunker guards with verified SNAFU/GCGN weapons.
3. Restore town patrols/capture AI.
4. Reintroduce verified My_DF_Gear classnames from `Delta_Force_Gear_Types`.
5. Reintroduce EoH intel loot only after item classnames are confirmed in config/mod source.
6. Reintroduce SNAFU attachments only from verified weapon attachment compatibility data.

---

# Completed / Working Systems

## EoH Notification Standard

- Added shared EoH notification helper:

```text
EoH_Server/scripts/4_World/EoH_Core/eoh_notifications.c
```

- Standardized global EoH alerts to use the same Expansion notification pattern proven by MerkZone KOTH:

```text
ExpansionNotification(title, message, "Territory", COLOR_EXPANSION_NOTIFICATION_MISSION, duration).Create();
```

- EoH systems should now use:

```text
EoH_Notifications.SendToAll(title, message)
EoH_Notifications.SendToPlayer(player, title, message)
```

- Intended for:
  - bunker alerts
  - CBD room alerts
  - town capture alerts
  - relay alerts
  - black market movement alerts
  - roaming trader discovery alerts
  - future EoH event systems

## Relay Gameplay System

- Relay objects spawning.
- Relay ownership/capture groundwork.
- Group-name integration.
- Radio-based respawn integration.
- Town respawn groundwork.
- Terje StartScreen integration groundwork.
- Relay startup config now defaults to cleaning existing EoH relays before spawning new relays to prevent restart stacking.

Updated:

```text
EoH_Server/scripts/3_Game/EoH_CaptureRelay/eoh_relay_config.c
```

Important config default:

```text
DeleteExistingEoHRelaysBeforeSpawn = true
```

Live server note:
- Existing profile config may still need manual update at:

```text
$profile:EoH_Server/Relays.json
```

Set:

```json
"DeleteExistingEoHRelaysBeforeSpawn": true
```

## EoH Build Control / No-Build Zones

- Added configurable no-build zone support.
- Config path:

```text
$profile:EoH_BuildControl/NoBuildZones.json
```

- Repo config example added:

```text
EoH_Server/config/EoH_BuildControl/NoBuildZones.json
```

- Script added:

```text
EoH_Server/scripts/4_World/EoH_BuildControl/eoh_nobuildzones.c
```

- Current implementation blocks deploy/build actions based on player position inside configured radius.
- Hologram collision override was removed because it was incompatible with the active DayZ 1.29/mod stack.

Recommended EoH use:
- trader zones
- bunker conflict zone
- CBD rooms/event zones
- capture towns
- relay objectives
- black market zones

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
- Safe recovery AI Bandits config created and validated live.
- Bunker AI safe groups confirmed spawning on both bunker levels.
- Green Mountain recovery test group removed from safe recovery config because it violated safe-zone trader gameplay.
- Tier 1 AI and drug trader guards should use ALV clothing classnames from the ALV clothing list.
- Drug trader guards should use mid-tier gear/weapons and be stronger than basic Tier 1 town AI.
- Black market AI should be just below bunker AI: elite-adjacent, dangerous, and well-equipped, but not carrying bunker-only/endgame reward weapons.
- Bunker AI remains the highest normal AI tier and should use the best verified combat gear/loadouts.

Validated recovery config:

```text
EoH_Server/config/AIB/DynamicAIB_EoH_SAFE_RECOVERY.json
```

Important note:
- Do not restore the old full DynamicAIB config all at once.
- Rebuild from the safe recovery file in phases with verified classnames.

### AI Difficulty / Loadout Tier Rules

```text
Tier 1 Town AI
- ALV civilian/scavenger/light-military clothing
- light to mid-tier weapons
- limited armor
- low grenade chance
- no bunker-tier weapons

Drug Trader Guards
- ALV darker/scavenger/militia clothing
- mid-tier weapons
- moderate armor
- slightly harder than Tier 1 town AI
- not equal to black market guards

Black Market Guards
- just below bunker AI
- high accuracy, strong armor, coordinated guard feel
- mid-high to high-tier weapons only
- no bunker-exclusive reward weapons
- no .50 cal / .408 Chey loadouts as normal guard weapons

Bunker AI
- highest standard AI difficulty
- bunker/endgame combat gear
- strongest verified weapons and armor
- protects epic loot and raid progression resources
```

## CBD / Key Rooms

- Converted toward CBD loot-room direction.
- Tiered room progression preserved.
- Punch cards restricted to Tier 4 loot rooms only.
- Key drop integration started.
- CBD observer now routes door-open markers through the unified CBD marker helper.
- CBD marker IDs were normalized to fix add/update/remove mismatch.
- Door-open behavior now sends:
  - flashing/pulsing keyroom marker
  - danger icon
  - tier-based color
  - upper-left EoH notification
- Marker cleanup now uses the same marker ID that was used to create the marker.

Updated:

```text
EoH_Server/scripts/4_World/eoh_cbd_observer.c
EoH_Server/scripts/4_World/eoh_cbd_markerhelper.c
```

Expected logs:

```text
[EoH_CBD] Door opened:
[EoH_CBD] Broadcast flash marker
[EoH_CBD] Cleared marker
```

## Bunker Systems

- Punch card ruin-on-use system added.
- Bunker global alert framework added.
- ActionUseUndergroundPanel bridge added.
- Debug logging added to bunker bridge.
- Panel observer fallback added to help detect bunker panel object/class safely.
- Bunker alert was converted to the shared EoH notification helper.
- Duplicate `EoH_BunkerGlobalAlert` class issue fixed by removing duplicate helper declaration.
- MissionServer observer hook moved to `5_Mission` where `MissionServer` exists.
- Unsafe `Object.GetAnimationPhase()` calls were removed from the panel observer.
- CJ187-LootChest bunker direction chosen.
- MMG containers should be treated as visual/decorative bunker props, while CJ187-compatible chest classes handle loot backend.
- Bunker AI now validated with safe recovery AIB config on both bunker levels.
- Bunker loot excludes filler categories:
  - no sidearms
  - no general supplies
  - no tools
  - no medical loot

## Bunker Loot Direction

Bunker loot focuses only on endgame combat and raid progression:

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

Seven hand-placed bunker containers are planned through CJ187-LootChest.

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

Bunker loot includes Breaching Charge mod crafting parts and rare completed charge items.

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
- Players should need multiple bunker runs to reliably build raid explosives.

## Expansion Market Updates

### Medical

Updated:

```text
Server_Config_Backup/HostHavocDayZServer/ExpansionMod/Market/Medical.json
```

- Added Terje medical-only items:
  - salves
  - pills
  - ampoules
  - injectors
  - surgical kits/tools
  - advanced medkits
  - diagnostics/tests
  - syringes
  - defibrillator
- Excluded normal-market poison/Novichek-style items.
- Balance direction:
  - basic meds common
  - advanced meds expensive
  - elite injectors rare

### Furnishings

Updated:

```text
Server_Config_Backup/HostHavocDayZServer/ExpansionMod/Market/Furnishings.json
```

- Added Terje sleeping bags:
  - `TerjeSleepingBag_Blue`
  - `TerjeSleepingBag_Woodland`
- Sleeping bags were intentionally placed in Furnishings instead of Medical.

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

# Current Known Issues / Validation Needed

## Relay Stacking

Code default has been changed to clean existing relays before spawn, but live server profile config may still need manual update:

```text
$profile:EoH_Server/Relays.json
```

Set:

```json
"DeleteExistingEoHRelaysBeforeSpawn": true
```

Need to validate that the actual relay spawner honors this flag.

## CBD Marker Flash / Global Alerts

Implemented and needs live validation.

Need to confirm:
- CBD observer detects door open.
- Marker appears for all players.
- Marker pulses/flashes.
- Marker clears on close.
- EoH notification appears upper-left.

## Bunker Notification

Bunker notification now uses shared EoH notification helper.

Need to confirm:
- notification appears upper-left using Expansion UI
- no global chat fallback is used when Expansion is active
- panel observer/action bridge do not double-fire

## Bunker Loot Config

CJ187 bunker loot now works but still needs gameplay balance validation.

Need to verify:
- crate fill quantity feels rewarding but not excessive
- explosive progression requires repeated bunker runs
- no invalid classnames remain
- no inventory overflow issues occur

## AI Config Rebuild

Safe recovery AIB config is validated. The old full DynamicAIB should not be restored as-is.

Need to rebuild:
- roaming trader escorts
- town/capture AI
- final bunker AI loadouts
- drug trader guards using ALV clothing
- black market guards just below bunker AI

using verified classnames only.

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

1. Rebuild roaming trader escorts from safe AIB recovery config.
2. Validate shared EoH notifications in live server.
3. Validate CBD marker flash after opening a CBD door.
4. Confirm relay cleanup flag is active in live `$profile:EoH_Server/Relays.json`.
5. Locate/verify actual relay spawner honors `DeleteExistingEoHRelaysBeforeSpawn`.
6. Rebuild town/capture AI after capture loop is working.
7. Continue bunker loot balance testing.
8. Remove bunker weapons from trader sell inventories.
9. Limit bunker ammo and magazines at traders/black market.
10. Resume high-quality handcrafted quest generation.
