# EoH Current State Changelog

## Current Focus

Live gameplay systems stabilization and weekly event framework buildout.

Primary current systems:

- Town capture ownership loop.
- Town AI proximity streaming.
- Terje town respawn ownership filtering.
- Roaming trader proximity/intel behavior.
- Town reward smoke validation.
- Territory flag respawn and private group flag markers.
- Weekly Event Objective framework.
- Altar Relay Towers weekend event.
- Purge Night weekend event.

---

# Latest Validation Notes

## Weekly Event Objective System / Altar Relay Towers

The old standalone Weekly Event prototype has been retired.

Removed legacy files / routes:

```text
EoH_Server/scripts/4_World/EoH_WeeklyEvents/EoH_WeeklyEventConfig.c
EoH_Server/scripts/4_World/EoH_WeeklyEvents/EoH_WeeklyEventManager.c
```

Scheduler now routes weekly events directly to:

```text
EoH_EventObjectiveManager.Get().Tick();
```

Active weekly-event framework:

```text
EoH_Server/scripts/4_World/EoH_WeeklyEvents/eoh_event_objective_manager.c
```

Live profile config:

```text
$profile:EoH/WeeklyEventConfig.json
```

Important config fields:

```json
{
  "AltarRepairDurationSeconds": 10.0,
  "AltarRepairProximityRadius": 5.0,
  "AltarRepairWatcherIntervalMs": 5000,
  "EnableAltarProximityAutoRepair": true
}
```

Current Altar event behavior:

```text
1. Player uses weekly event intel.
2. Force-test currently selects Altar Relay Towers.
3. Event marker broadcasts at the placed Altar relay position.
4. Dedicated Altar repair watcher starts.
5. Watcher checks players near relay every 5 seconds.
6. If player has Field Transceiver + Car Battery:
   - If EnableAltarProximityAutoRepair=true, relay auto-repairs.
   - If false, player only receives prompt text asking for terminal action.
7. Repair consumes radio + battery.
8. Relay goes online.
9. Reward crate spawns.
10. Red smoke spawns.
11. Wave 1 spawns.
12. RELAY ONLINE notification broadcasts.
```

Validated live:

```text
[EoH_AltarRepair][PROXIMITY] player=Beck/wfbeck dist=<5 hasRadio=1 hasBattery=1
[EoH_Notifications] RELAY REPAIR - Field Transceiver and Car Battery consumed. Relay uplink restored.
[EoH_EventObjectives] Reward crate spawned id=altar_relay_towers
[EoH_EventObjectives] Spawned event grenade smoke type=M18SmokeGrenade_Red
[EoH_EventWaves] Spawning wave=1 objective=altar_relay_towers
[EoH_Notifications] RELAY ONLINE - Altar Relay Towers has been restored.
```

Important note:

```text
No visible DayZ repair action is currently required or expected if auto repair is enabled.
The action-based approach was abandoned because the placed DZE terminal is a static/builder object and action registration did not fire reliably.
```

Current required server-side setting:

```text
$profile:EoH/WeeklyEventConfig.json
EnableAltarProximityAutoRepair=true
```

If this value is false, the logs will show:

```text
autoRepair=false
Repair equipment detected. Use the relay terminal action to restore the uplink.
```

and the event will not start automatically.

## Altar Relay Position Fix

The original Altar event position was several meters away from the actual placed relay object.

Old position:

```text
8132.95068359375 492.1257629394531 9093.74609375
```

Updated position:

```text
8130.686035 492.125732 9085.325195
```

This position was validated from live player position while standing directly at the placed item.

## Altar / Purge Reward Crate Notes

Reward crate and smoke spawn successfully for Altar repair.

Several prior reward classnames failed and were replaced with safer fallback items:

```text
PunchedCard
DNA_Keycard_Red
M4A1
AKM
Mag_STANAG_30Rnd
Mag_AKM_30Rnd
Ammo_556x45
Ammo_762x39
PlateCarrierVest
NVGoggles
EoH_TownIntel
EoH_TraderIntel
```

Need to validate the exact desired modded reward classnames later before reintroducing My Delta Force/SNAFU-specific items.

## Purge Night Event Status

Purge Night event loop has been built and previously validated for:

- Reward crate spawn.
- Green smoke spawn using grenade/ParticlePoints fallback path.
- Reward crate cleanup.
- Marker cleanup.
- Intel marker behavior.
- AI spawn working after loadout updates.
- Live weekend timing design:

```text
Event duration: 30 minutes
Reward window: 10 minutes
Total event lifecycle: about 40 minutes
```

Need to revalidate after latest weekly-event cleanup.

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

Town reward smoke was eventually validated for Purge Night reward after ParticlePoints mod path was installed and integrated.

Still worth revalidating:

- Town capture reward smoke.
- Altar red repair smoke.
- Purge green reward smoke.
- Cleanup of smoke grenade/ParticlePoint objects.

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

- Standardized global EoH alerts to use the same Expansion notification pattern proven by MerkZone KOTH.

EoH systems should use:

```text
EoH_Notifications.SendToAll(title, message)
EoH_Notifications.SendToPlayer(player, title, message)
```

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
- Weekly event intel now reveals Altar Relay Towers during force-test mode.

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
- Current recommendation: AI_Bandits/DynamicAIB should remain bunker-only; EoH Town AI should own town/capture AI.

## Raid / Breaching Direction

Confirmed fixed during testing:

- Breaching charge placement works again.
- CodeLock compatibility is active.
- BBP config supports door-only style raiding:

```text
BBP_RaidOnlyDoors = 1
```

Current raid design direction:

```text
Raidable: doors, gates, hatches
Not raidable: walls, floors, foundations, roofs, stairs, pillars, windows
```

## Relay Gameplay System

- Relay objects spawning.
- Relay ownership/capture groundwork.
- Group-name integration.
- Radio-based respawn integration.
- Town respawn groundwork.
- Terje StartScreen integration groundwork.
- Relay startup config now defaults to cleaning existing EoH relays before spawning new relays to prevent restart stacking.

---

# Current Known Issues / Validation Needed

## Weekly Event Objective Framework

Need to validate:

- `EnableAltarProximityAutoRepair=true` on live profile config.
- Altar event starts automatically after proximity check.
- Altar Wave 1 AI spawns and engages.
- Altar Waves 2 and 3 arrive on schedule.
- Altar reward crate inventory contains intended items.
- Altar event cleanup at end of duration.
- Purge Night still works after legacy weekly manager removal.

## Reward Loot Classnames

Need to audit exact classnames for:

- Single-use punch card.
- DNA red keycard.
- My Delta Force weapons.
- SNAFU weapons.
- My Delta Force gear.
- Correct magazine/ammo classnames.

Current fallback crate rewards are intentionally conservative until classnames are confirmed.

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

---

# Recommended Next Session Priorities

1. Set and confirm `$profile:EoH/WeeklyEventConfig.json` has `EnableAltarProximityAutoRepair=true`.
2. Retest Altar Relay full loop: intel -> proximity repair -> item consumption -> crate/smoke -> waves -> cleanup.
3. Validate Purge Night still works after removal of legacy WeeklyEventManager.
4. Audit reward crate classnames and reintroduce correct SNAFU/MDF/punchcard/keycard rewards.
5. Wire `EoH_GroupFlagMarkerManager.Tick()` into an existing heartbeat.
6. Continue validating town capture reward smoke and cleanup.
7. Add or polish the next weekend event.
8. Validate trader intel performance after removing immediate forced spawn.
9. Add contested-town and enemy-nearby respawn lockouts.
10. Resume high-quality handcrafted quest generation.
