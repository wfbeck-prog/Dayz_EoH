# EoH Current State Changelog

## Current Focus

Live gameplay systems stabilization.

Primary current systems:

- Town capture ownership loop.
- Town AI proximity streaming.
- Terje town respawn ownership filtering.
- Roaming trader proximity/intel behavior.
- Town reward smoke validation.
- Territory flag respawn and private group flag markers.

---

# Latest Validation Notes

## Town Respawn / Terje Start Screen

Confirmed live:

- Static Terje XML town respawns are selectable.
- Runtime/dynamic XML injection was tested and abandoned because injected entries appeared but remained unavailable.
- Dynamic Start Screen injection file is now disabled and kept only as documentation:

```text
EoH_Server/scripts/4_World/EoH_Terje/EoH_TerjeStartScreenPlugin.c
```

- Static XML entries for all 18 capture towns now work.
- Ownership filtering works: all towns may display, but only towns owned by the player's group are usable.
- Safe-offset town respawn XML was added so players do not respawn directly on capture relays/radios.

Safe-offset XML helper:

```text
Server_Config_Backup/HostHavocDayZServer/StartScreen/EoH_TownRespawn_SafeOffsets.xml
```

Current town respawn behavior:

```text
Owned town = usable respawn option
Unowned town = unavailable option
Spawn point = offset from relay/capture center
```

## Roaming Trader Intel / Proximity Streaming

Confirmed live:

- Roaming traders and escorts now use player-proximity activation.
- Trader intel can reveal an inactive trader route node.
- Trader intel notification works.
- Trader intel marker works.
- Trader intel no longer requires the trader object to already be spawned.

Performance update:

- Immediate forced trader activation during intel use was removed because it could trigger a server freeze/hitch.
- The suspected source was a large world object scan used to locate pre-placed trader NPCs.
- Intel now reveals marker/notification first and allows normal update logic to activate traders later.

## Town AI Proximity Streaming

Confirmed live:

- Town AI no longer globally activates all towns.
- Town AI only activates towns near players.
- Server FPS returned above 3000 after proximity-streaming changes.
- Tier 4 areas are available but no longer spawn everywhere at once.

## Town Capture Validation

Confirmed live during Chernogorsk capture test:

- Capture progress updates correctly.
- Capture marker broadcasts correctly.
- Capture notification broadcasts correctly.
- Presence/contested logic reports correctly.
- Only the nearby town activates Town AI while other towns are skipped by proximity.

## Reward Smoke Status

Still under validation:

- Reward crate spawns.
- Reward smoke marker object has spawned in prior tests.
- Visible smoke is still not reliable.
- Reward smoke marker now retries client-side particle startup at multiple timings:

```text
250ms
1s
2.5s
5s
```

Updated file:

```text
EoH_Server/scripts/4_World/EoH_TownReward/eoh_reward_smoke_marker.c
```

Expected smoke debug logs:

```text
[EoH_TownReward][SMOKE] Client started reward smoke particle
[EoH_TownReward][SMOKE][WARN] Particle.PlayOnObject returned null
[EoH_TownReward][SMOKE][WARN] Reward smoke marker client has invalid position
```

## Territory Flag / Group Marker Progress

Added first version of private group flag marker manager:

```text
EoH_Server/scripts/4_World/EoH_Terje/EoH_GroupFlagMarkerManager.c
```

Purpose:

```text
Group members can see their own TerritoryFlag marker.
Non-members should not see the marker.
```

Current status:

- Manager exists.
- Uses group ownership checks against TerritoryFlag ownership.
- Sends personal/private marker category instead of global Expansion server marker.
- Still needs to be wired into an existing heartbeat with:

```text
EoH_GroupFlagMarkerManager.Tick();
```

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
- Static XML town respawns confirmed selectable.
- Owned-town condition/ownership filtering confirmed working live.
- Safe-offset XML added for captured-town respawns.

## Intel System

- Intel books/items created.
- Intel groundwork tied toward future Expansion Book integration.
- Book manager framework started.
- Trader intel now supports inactive roaming trader route reveal.
- Trader intel no longer depends on a spawned trader object.

## AI / Combat Systems

- Town AI progression.
- AI loot integration.
- SNAFU + MDF loadout direction.
- Tier 3.5 boss AI templates created:
  - SKSuperman
  - All Out Genius
  - DeadlyDead SK
- Boss quest objective/loadout update pack generated for the final three AI quest bosses.
- Safe recovery AI Bandits config created and validated live.
- Bunker AI safe groups confirmed spawning on both bunker levels.
- Green Mountain recovery test group removed from safe recovery config because it violated safe-zone trader gameplay.
- Town-control gameplay loop remains planned around 4 AI levels/tiers.
- Town AI now uses player proximity activation to avoid global AI load.
- Roaming traders now use player proximity activation for trader/escort load.
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
- Current recommendation: AI_Bandits/DynamicAIB should remain bunker-only; EoH Town AI should own town/capture AI.

### Town-Control AI: 4-Level Loop

Town control should use four escalating AI levels tied to town value and capture progression:

```text
Level 1 - Outer Town Patrols
- ALV civilian/scavenger/light-military clothing
- low-mid accuracy
- light to mid-tier weapons
- light armor / chest rigs
- low grenade chance
- intended for Tier 1 towns and early capture pressure

Level 2 - Armed Town Defenders
- ALV militia/light military clothing
- medium accuracy
- mid-tier rifles
- moderate armor
- better loot than Level 1
- used for stronger towns and contested captures

Level 3 - Veteran Occupation Forces
- heavier armor and more coordinated patrols
- mid-high tier weapons
- higher accuracy
- controlled grenade chance
- protects high-value towns and mature captures

Level 4 - Elite Town Command / Boss Defense
- strongest town-control AI tier
- boss/commander style presence
- high accuracy and armor
- high-tier weapons, but still below bunker-exclusive reward pool
- should feel like a major objective without replacing bunker/endgame loot progression
```

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

## Terje Respawn Remaining Work

Need to validate:

- `EoHOwnedTown` condition continues to work after latest builds.
- Safe-offset town respawn points feel fair and do not place players in bad terrain/water.
- Territory flag respawn still works for owned flags.
- Non-owner flag respawn is denied.
- Whether flag respawn still requires manual set/bind behavior.

Future polish:

- Add cooldown back to town respawns if desired.
- Add contested-town lockout.
- Add enemy-nearby lockout.
- Consider hiding unavailable towns rather than showing all towns as unavailable.

## Group Flag Marker Hook

`EoH_GroupFlagMarkerManager` exists but needs to be called by an update loop.

Need to add/test:

```text
EoH_GroupFlagMarkerManager.Tick();
```

## Reward Smoke

Town reward smoke remains the most visible unresolved issue.

Need to confirm with latest PBO:

- Does client print smoke startup log?
- Does `Particle.PlayOnObject` return null?
- Does marker object exist client-side?
- Should the next implementation use a dedicated client RPC instead of relying on marker object lifecycle?

## Expansion AI Inventory Exceptions

Some Expansion AI loadouts still occasionally trigger inventory clone/take-to-hands exceptions:

```text
Function GetInstanceID
Expansion_CloneItemToLocation
EAI take item to hands/inventory
```

Most suspicious active loadouts observed during testing:

```text
EoH_AI_Patrol_Marksman_DF
EoH_AI_Drug_SMGS
```

Need to audit those loadouts for unsupported/invalid/attachment-heavy items.

## Trader Intel Performance

Immediate trader spawn after intel reveal was removed to avoid hitch/freeze.

Need to validate:

- Intel notification still appears.
- Intel marker still appears.
- Trader still activates naturally when players approach.
- Marker persistence feels long enough and does not clear too soon.

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
- final bunker AI loadouts
- verify roaming trader escort loadouts against Expansion AI inventory errors
- verify town/capture AI loadouts against Expansion AI inventory errors
- drug trader guards using ALV clothing
- black market guards just below bunker AI
- four-level town-control AI loop

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

1. Validate safe-offset town respawn XML in live server.
2. Wire `EoH_GroupFlagMarkerManager.Tick()` into an existing heartbeat.
3. Continue debugging town reward smoke with the new retry logs.
4. Add contested-town and enemy-nearby respawn lockouts.
5. Validate owned TerritoryFlag respawn behavior.
6. Audit Expansion AI loadouts causing inventory clone exceptions.
7. Validate trader intel performance after removing immediate forced spawn.
8. Confirm relay cleanup flag is active in live `$profile:EoH_Server/Relays.json`.
9. Continue bunker loot balance testing.
10. Resume high-quality handcrafted quest generation.
