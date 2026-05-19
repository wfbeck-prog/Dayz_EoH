# Changelog

All notable EoH_Server repository changes are tracked here.

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
