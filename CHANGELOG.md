# Changelog

All notable EoH_Server repository changes are tracked here.

## 2026-06-06

### Added
- Added restart-safe weekly event persistence validation for Altar Relay Towers.
- Added recovery runtime reconstruction for active weekly events after server restart.
- Added recovery grace support for active weekly events:
  - `EnableRecoveryGracePeriod`
  - `RecoveryGraceSeconds`
- Added passive recovery mode so restored events do not immediately spawn AI, advance waves, or spawn rewards before reconnect grace expires.
- Added script-log and Live Advisor recovery diagnostics:
  - `[EoH_Recovery] grace_active`
  - `[EoH_Recovery] runtime_rebuilt`
  - `[EoH_Recovery] grace_expired`
  - `[EoH_Recovery] passive_released`
- Added weekly event state heartbeat helper:
  - `EoH_WeeklyEventStateDebug`
  - `[EoH_WeeklyEventState] state active=... objective=... wave=... repaired=... passive=... restored=... grace=...`
- Added restored-runtime grace handling to prevent the Altar 75m abandonment rule from falsely resetting a repaired event during server restart/relog.

### Changed
- Changed restored Altar Relay events to rebuild from `WeeklyEventRuntimeState.json` instead of remaining only as saved persistence data.
- Changed recovery flow so a rebuilt event starts in `RecoveryPassiveMode=true` during grace.
- Changed Phase 3C recovery flow so passive mode releases after grace expires and the event can continue from the saved wave state.
- Changed recovery visibility to log both to script output and Live Advisor where possible.

### Fixed
- Fixed active weekly event state surviving restart but not reconstructing usable runtime state.
- Fixed Altar Relay recovery reset risk caused by players being disconnected during restart while the 75m maintain-radius rule is enabled.
- Fixed missing visibility into whether an active event is normal runtime or recovered passive runtime.
- Fixed weekly event state debugging string syntax by replacing multi-line chained concatenation with safer incremental string assembly.

### Verified
- Verified fresh intel can start Altar Relay after persistence is idle.
- Verified live runtime heartbeat reports normal active state:
  - `passive=false`
  - `restored=false`
  - `grace=false`
- Verified restart recovery rebuilt Altar Relay Towers from persistence:
  - `repaired=true`
  - `wave=4`
  - `rewardUnlocked=false`
  - `passive=true`
  - `restored=true`
  - `grace=true`
- Verified the recovered marker and broadcast are restored after reboot.

### Notes
- Phase 3B is confirmed working: runtime rebuild, marker restore, repaired state restore, saved wave restore, and reconnect grace all survive restart.
- Phase 3C is now staged for validation: after the 300-second grace window, passive mode should release and the event should continue toward the next saved wave/reward step.
- The large `eoh_event_objective_manager.c` should not be rewritten through large single-payload connector updates. Use local full-file edits or small helper files to avoid truncation/clipping.

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
