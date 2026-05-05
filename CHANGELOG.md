# Changelog

All notable EoH_Server repository changes are tracked here.

## 2026-05-05

### Added
- Added unified marker framework under `EoH_Server/scripts/3_Game/EoH_Markers` and `EoH_Server/scripts/4_World/EoH_Markers`.
  - `EoH_MarkerData`
  - `EoH_MarkerService`
  - `EoH_MarkerManager`
  - `EoH_PlayerMarkerRPC`
- Added `EoH_Server/config/AIB/DynamicAIB_EoH_Template.json` as the maintainable AIB/DynamicAI source template for:
  - manual custom AI patrols/snipers
  - trader guard groups
  - town capture AI groups
  - reusable predefined weapon pools
- Added spawnable EoH config aliases/classes in `EoH_Server/config.cpp` for:
  - `EoH_RadioRelay`
  - `EoH_TownIntel`
  - `EoH_TraderIntel`
  - `EoH_Intel_Document`
  - `EoH_CaptureRelay_Base`
- Added concrete intel item script classes in `eoh_intel_item.c` for:
  - `EoH_Intel_Document`
  - `EoH_TownIntel`
  - `EoH_TraderIntel`
- Added `EoH_FlagOwnership.c` to extend `Flag_Base` with EoH group ownership helpers.
- Added RoamingTrader files into `EoH_Server` so the server can move toward one consolidated mod.
- Added RoamingTrader PlayerBase proximity reveal hook so finding a trader can reveal its marker.

### Changed
- Merged RoamingTrader config definitions into `EoH_Server/config.cpp`.
- Restored RoamingTrader Expansion Market class usage:
  - drug trader uses `ExpansionTraderChem`
  - black market trader uses `ExpansionTraderBroker`
- Converted town marker manager into a wrapper around the unified marker service.
- Converted intel marker reveal to use `EoH_MarkerService` instead of old raw marker/RPC logic.
- Converted MissionServer base town markers to use unified marker data instead of old raw RPC `777001`.
- Wired `MissionServer` tick loop to update both capture manager and RoamingTrader manager.
- Added `GetTownOwner()` wrapper to `EoH_CaptureManager`, reading persistent ownership from `EoH_WorldStateManager`.
- Updated BuildControl ownership checks to use `Flag_Base.GetEoHOwner()` instead of calling ownership helpers through `TerritoryFlag`.
- Updated `TerritoryFlagKit` ownership assignment to cast created flag object to `Flag_Base` before calling `SetEoHOwner()`.
- Changed RoamingTrader AI integration so EoH_Server no longer spawns raw/naked AI. Trader/town AI is now intended to be maintained through `DynamicAI.json` / the EoH DynamicAIB template.
- Removed invalid punched card action overrides and left punched card action registration safe for compile.
- Removed invalid guessed Terje hook based on nonexistent `TerjeSpawnSelector` class.

### Removed
- Removed obsolete duplicate marker and client marker systems:
  - `eoh_marker_manager.c`
  - `eoh_town_marker_data.c`
  - `eoh_playerbase_markers.c`
  - `eoh_client_marker_store.c`
  - `eoh_marker_client.c`
  - `eoh_client_mapmenu.c`
  - `eoh_mapmenu_markers.c`
  - `eoh_intel_rpc.c`
- Removed duplicate `EoH_GroupHelper` declaration from the older BuildControl location.
- Removed duplicate relay class declaration from `eoh_intel_item.c`; relay remains owned by `eoh_capture_relay.c`.

### Fixed
- Fixed multiple DayZ compile blockers during EoH_Server consolidation:
  - duplicate class declarations
  - invalid action overrides
  - invalid raw RPC marker handlers
  - stale references to deleted marker classes
  - `TerritoryFlag.SetEoHOwner` / `GetEoHOwner` undefined calls
  - `EoH_IntelManager.ResetPlayerIntelUsage` missing
  - `EoH_IntelManager.RevealTownIntel` missing
  - `EoH_CaptureManager.GetTownOwner` missing
  - MissionServer multi-line RPC syntax failure
- Fixed RoamingTrader reveal architecture so each trader reveal is tracked independently by trader ID.
- Fixed intel manager to reset player intel usage on death and send per-player intel markers through the unified marker system.

### Known follow-ups
- Verify admin-spawned `EoH_TownIntel`, `EoH_TraderIntel`, and `EoH_RadioRelay` after rebuilding PBO.
- Verify RoamingTrader marker reveal after rebuilding PBO and approaching each trader.
- Replace square marker icons by using exact Expansion icon names:
  - town markers: `Territory`
  - trader markers: `Trader`
  - intel markers: `Info`
  - radio markers: `Radio`
- Rebuild proper Terje territory respawn integration using real Terje StartScreen classes from uploaded reference files.
- Decide whether bunker punchcard event should hook via exact `ActionUseUndergroundPanel` method from the installed Expansion/DayZ version.
- Maintain live AIB guards manually in server `DynamicAI.json` using `DynamicAIB_EoH_Template.json` as the source template.
