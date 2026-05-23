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
- Weekly Relay Collapse event scaffold.

---

# Latest Validation Notes

## Weekly Event System / Relay Collapse Scaffold

Started the EoH Weekly Event framework based on the planned Relay Collapse and Purge Night design.

Added config:

```text
EoH_Server/scripts/4_World/EoH_WeeklyEvents/EoH_WeeklyEventConfig.c
```

Creates live profile config:

```text
$profile:EoH_Server/WeeklyEvents.json
```

Added manager scaffold:

```text
EoH_Server/scripts/4_World/EoH_WeeklyEvents/EoH_WeeklyEventManager.c
```

Current Relay Collapse lifecycle:

```text
IDLE
WARNING
ACTIVE
SUCCESS
RESET
```

Current scaffold functionality:

- Weekly event config load/create.
- Relay Collapse enable flag.
- Purge Night placeholder flag.
- Warning duration config.
- Active duration config.
- Random relay-town selection placeholder.
- EoH global notifications for warning/active/success.
- Debug logs for event lifecycle.

Still needed:

- Wire `EoH_WeeklyEventManager.Get().Tick();` into server heartbeat.
- Replace placeholder town selection with real relay/live relay selection.
- Add event markers.
- Add AI occupation spawns.
- Add objective/completion handling.
- Add persistence/cooldown.
- Add Purge Night after Relay Collapse is validated.

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

## Weekly Event Framework

Need to validate:

- WeeklyEvents.json generates under `$profile:EoH_Server`.
- Relay Collapse notifications fire correctly after heartbeat wiring.
- Event state transitions work without rapid looping.
- Placeholder relay-town selection should be replaced with live relay data.

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

---

# Recommended Next Session Priorities

1. Wire `EoH_WeeklyEventManager.Get().Tick()` into server heartbeat.
2. Validate WeeklyEvents.json creation and Relay Collapse notification lifecycle.
3. Wire `EoH_GroupFlagMarkerManager.Tick()` into an existing heartbeat.
4. Continue debugging town reward smoke with the new retry logs.
5. Add Relay Collapse event marker.
6. Replace placeholder event town picker with live relay data.
7. Add Relay Collapse AI occupation spawns.
8. Add contested-town and enemy-nearby respawn lockouts.
9. Validate trader intel performance after removing immediate forced spawn.
10. Resume high-quality handcrafted quest generation.
