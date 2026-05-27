# Changelog

All notable EoH_Server repository changes are tracked here.

## 2026-05-27

### Added
- Added `Purge Night: Central Corridor` as the second EoH weekend event direction.
- Added temporary forced Purge Night test mode:
  - `EOH_FORCE_PURGE_NIGHT_TEST = true`
- Added 5-minute Purge Night test duration while forced test mode is enabled.
- Added Purge Night phase timing for test mode:
  - Phase 1 at 1 minute
  - Phase 2 at 3 minutes
  - Phase 3 at 4 minutes
  - Reward unlock at 5 minutes
- Added Purge Night-specific field report:
  - `PURGE NIGHT INTERCEPT`
  - `Red Ledger Blackout Signal`
- Added Purge Night runtime diagnostics:
  - `[EoH_PurgeNight][TICK]`
  - elapsed time
  - duration
  - phase thresholds
  - current phase/wave
- Added weekend one-time-use runtime tracking so each event can only start once per server session for now.
- Added Purge Night recovery-cache fallback messaging so the event remains playable while smoke is unresolved.
- Added explicit `RECOVERY CACHE` broadcast when the Purge Night reward becomes vulnerable.
- Added smoke success/failure logging for event reward unlocks.

### Changed
- Changed weekly event intel testing so `EoH_AltarRelayIntel` currently forces Purge Night instead of random event selection.
- Changed Purge Night reward crate placement to force terrain surface height instead of using the event marker Y value.
- Changed event smoke spawning to return true/false for diagnostics.
- Changed Purge Night completion messaging to include a no-smoke fallback when smoke confirmation fails.
- Changed Altar Relay report wording from `Field Radio` to `Field Transceiver` where appropriate.
- Cleaned `EoH_AI_Patrol_Assault_DF.json` for Expansion AI stability testing:
  - removed magazines attached directly to weapons
  - kept loose reload magazines in cargo
  - removed duplicate/legacy intel and keycard class names
- Cleaned `EoH_AI_Patrol_Marksman_DF.json` using the same Expansion AI-safe pattern.

### Fixed
- Fixed Purge Night reward sea chest floating by snapping the reward crate to `GetGame().SurfaceY()`.
- Fixed Purge Night intel showing the old Altar Relay field report after Purge Night started.
- Fixed missing active objective config accessor used by weekly event intel reports.
- Fixed event reward completion path so it still broadcasts useful recovery-cache information even when smoke fails.

### Notes
- Purge Night is currently in test mode and should be returned to 60 minutes before normal weekend use.
- ParticlePoints smoke support is planned later, likely using `SmokePoint_3`, but should not be enabled until the mod is installed on the server and client mod list.
- Expansion AI reload/inventory VM exceptions are currently parked for later troubleshooting. The issue is separate from Purge Night and has been observed when town patrols use `EoH_AI_Patrol_Assault_DF`.
- Current Purge Night test flow:
  1. Use `EoH_AltarRelayIntel`.
  2. Confirm Purge Night marker appears.
  3. Confirm Purge Night field report appears.
  4. Confirm reward crate is on the ground.
  5. Confirm `[EoH_PurgeNight][TICK]` logs every 30 seconds.
  6. Confirm phase broadcasts at 1/3/4 minutes.
  7. Confirm `RECOVERY CACHE` broadcast at 5 minutes.
  8. Smoke may still fail until ParticlePoints fallback is added.

## 2026-05-25

### Added
- Added `EoH_AltarRelayIntel` as a dedicated weekend-event intel item.
- Added Altar Relay Towers as the first EoH weekly/weekend event objective.
- Added event-driven weekly objective runtime architecture.
- Added active objective state gating so only one weekly operation can be active at a time.
- Added intel-driven event reveal flow for Altar Relay Towers.
- Added lore-driven Altar Relay field report.
- Added global intel lock while a weekly operation is active.
- Added lore-driven blocked-intel notification:
  - `INTEL CHANNELS DOWN`
  - `The relay network is dark. Restore the tower terminal before decoding more field intel.`
- Added `EoH_RelayRepairTerminal` as a dedicated tower repair object.
- Added terminal-based repair interaction:
  - `Repair Tower Terminal`
- Added repair requirements for the tower terminal:
  - `BaseRadio`
  - `CarBattery`
- Added staged weekly event runtime phases:
  - intel reveal
  - terminal repair
  - relay activation
  - hostile wave start
  - reward crate staging
  - extraction unlock
- Added M18 smoke grenade object support for event signaling.
- Added red smoke when the relay is repaired and hostile contact begins.
- Added green smoke when the reward cache becomes vulnerable.

### Changed
- Changed event intel so it now activates the weekly objective runtime instead of only displaying a report/marker.
- Changed event flow so combat does not begin from intel use alone.
- Changed event flow so Wave 1 begins only after tower terminal repair.
- Changed reward crate timer so it starts only after tower terminal repair.
- Changed non-event intel behavior so trader, town, and loot-cache intel are blocked while a weekly event is active.
- Changed relay repair design from a held `BaseRadio` action to a terminal-based world interaction.
- Changed repair wording from generic relay repair to tower terminal repair.
- Changed event smoke implementation away from unsupported direct particle constants.
- Changed smoke implementation to spawn real `M18SmokeGrenade_*` objects and activate them with `GetCompEM().SwitchOn()`.
- Changed `EoH_RelayRepairTerminal` script inheritance to match its config inheritance from `Radio`.

### Fixed
- Fixed issue where trader intel could still reveal roaming traders after event intel was used.
- Fixed issue where event intel did not create active objective runtime state.
- Fixed issue where duplicate event marker behavior could occur from separate intel marker and objective marker paths.
- Fixed issue where `FieldTransceiver` was used as a required class name instead of the actual `BaseRadio` class.
- Fixed unsupported `ParticleList.SMOKEGRENADE_RED` usage by replacing it with M18 smoke grenade objects.
- Fixed terminal script/config inheritance mismatch that allowed the repair terminal to behave like a normal movable radio.

### Performance / Architecture
- Preserved performance-first event architecture.
- Avoided world scans for event state.
- Kept event state runtime-driven and scheduler-safe.
- Continued separating weekly events from town capture ownership systems.
- Kept intel, event activation, reward unlock, and cleanup responsibilities separated.

### Notes
- Current Altar Relay Towers event test flow:
  1. Use `EoH_AltarRelayIntel`.
  2. Confirm Altar Relay Towers event marker appears.
  3. Confirm other intel reports `INTEL CHANNELS DOWN` while the operation is active.
  4. Go to the repair terminal near Altar.
  5. Carry `BaseRadio` and `CarBattery`.
  6. Use `Repair Tower Terminal`.
  7. Confirm relay activation, red smoke, Wave 1, reward timer, and later green smoke on extraction unlock.

## 2026-05-19

### Added
- Added personal quest travel overlay framework for Expansion quest travel/search objectives.
- Added client-side Expansion map circle drawing for quest search areas using a canvas overlay approach.
- Added circle-only quest travel overlays so players see a search area instead of an exact task marker icon.
- Added config-driven quest travel overlay support:
  - `$profile:EoH/QuestTravelOverlayConfig.json`
  - `QuestID`
  - `ObjectiveID`
  - `Label`
  - `TruePosition`
  - `Radius`
- Added startup creation/loading for the quest travel overlay config file.
- Added config validation/debug logging for quest overlay entries.
- Added automatic offset logic so `TruePosition` stores the real objective location while the visible circle center is offset.
- Added `EoH_FieldReportData` and field report service groundwork for future readable report UI support.
- Added DF-based AI loadout variation pack for mission AI patrols and camps:
  - patrol assault
  - patrol scout
  - patrol marksman
  - camp guard
  - camp heavy
  - camp raider
- Added config model groundwork for town reward tiers and loot pools.
- Added generated `TownRewardConfig.json` profile example under `ProfileConfigs/EoH/`.

### Changed
- Repurposed `EoH_TownIntel` from map-marker reveal intel into a Town Risk Report system.
- Removed town intel marker creation/removal behavior that could interfere with faint world markers and player intel markers.
- Town intel now evaluates the nearest known capture/settlement location instead of broadcasting multiple nearby markers.
- Town intel now reports the town owner/control state:
  - owned group name when captured
  - `Unclaimed` when no owner exists
- Town intel now performs live counts inside the town capture radius for:
  - infected
  - AI / bandits
  - online survivors
- Town intel now derives infected presence and human threat from live count data instead of pure random flavor.
- Town intel keeps medical opportunity and intel confidence as semi-randomized assessment fields for now.
- Town intel report delivery is currently handled through the stable notification path after custom UI testing proved unreliable in the current build.
- Updated AI quest objective loadout references to rotate between the new EoH DF patrol/camp loadouts instead of using the same loadout everywhere.
- Converted town reward direction from hardcoded town tiers/loot toward config-driven reward tiering.
- Town reward cache behavior now supports smoke-marked cache drops, improved surface placement, cleanup of old reward crates/smoke, and DF-focused non-bunker reward loot.
- Kept `EoH_TraderIntel` behavior unchanged.

### Fixed
- Fixed quest circle rendering compatibility issues:
  - removed unsupported Expansion marker radius assumptions
  - replaced point/icon-only marker behavior with a real map canvas circle overlay
  - corrected `MapToScreen` usage for the current DayZ/Expansion build
  - removed exact icon marker display for travel objectives
- Fixed quest overlay config parsing by changing `TruePosition` to a safe DayZ vector string format:
  - expected format: `"3060 310 7870"`
- Fixed quest overlay loader to convert `TruePosition` strings with `.ToVector()`.
- Fixed field report construction syntax issues in town intel.
- Removed incompatible custom UI hooks/overrides discovered during server testing.

### Assessment
- Town AI is **not fully implemented yet**.
- Current AI-related implementation includes:
  - mission AI loadout variation for AIPatrol/AICamp objectives
  - town intel live counting of AI/bandits inside the capture radius
  - capture/reward systems that can react to town ownership state
- A full Town AI system still needs:
  - `TownAIConfig.json`
  - town-to-tier AI spawn rules
  - defender/patrol/camp templates per town or tier
  - loadout rotation using the new DF AI loadouts
  - spawn/despawn cleanup logic
  - capture-state integration for unclaimed, owned, contested, and recently captured towns
  - safeguards to prevent AI stacking or abandoned persistent spawns

### Notes
- Quest travel overlays are now stable enough for multiplayer testing.
- For quest overlays, enter the exact task/objective location in `TruePosition`; the mod offsets the displayed circle automatically.
- The town intel system is intentionally being left at the current stable notification-report stage.
- Future expansion options may include:
  - full EoH field-report UI
  - medical lead generation from real loot/building scans
  - per-player randomized quest circle offsets
  - local scouting objectives
  - regional intel chains
  - hidden survivor encounters
  - full config-driven Town AI defenders/patrols by town tier and ownership state

## 2026-05-12

### Added
- Added DNA Keycards migration support for the new EoH loot-cache progression direction.
- Added DNA loot cache marker helper:
  - `EoH_Server/scripts/4_World/EoH_LootIntel/eoh_dna_cache_markerhelper.c`
- Added DNA loot cache open bridge:
  - `EoH_Server/scripts/4_World/EoH_LootIntel/eoh_dna_cache_openbridge.c`
- Added tier-aware DNA cache marker support:
  - Yellow = T1 Cache
  - Green = T2 Cache
  - Blue = T3 Cache
  - Red = T4 Cache
  - Purple = bunker-only / special cache direction
- Added temporary pulsing cache-open markers with timed removal support.
- Added marker framework support for:
  - `EoH_MarkerCategory.CACHE`
  - `EoH_MarkerState.ACTIVE`
- Added dogtag ledger Discord bridge:
  - `EoH_Server/scripts/4_World/EoH_Dogtags/eoh_dogtag_ledger.c`
- Added server status Discord bridge:
  - `EoH_Server/scripts/4_World/EoH_Status/eoh_server_status_webhook.c`
- Added delayed server-online webhook call from MissionServer startup:
  - `EoH_Server/scripts/5_Mission/eoh_server_status_mission.c`
- Added Red Ledger / high-value kill webhook support.
- Added server status webhook payload details:
  - online survivor count
  - in-game server time
- Added Dogtag and Server Status fields to `EoH_Webhooks.json` generation.
- Added High Value Kill webhook fields to `EoH_Webhooks.json` generation.
- Added combined My Delta Force durability override mod/config work under `EoH_DeltaForceDurability`.
- Added planning issue for evaluating replacement of CBD Loot Rooms, CJ187/JD LootChests, and BS KeyRoom remnants with DNA Keycard Crates.
