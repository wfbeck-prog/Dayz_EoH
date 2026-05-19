# Changelog

All notable EoH_Server repository changes are tracked here.

## 2026-05-19

### Changed
- Repurposed `EoH_TownIntel` from map-marker reveal intel into a text-based Town Risk Report system.
- Removed town intel marker creation/removal behavior that could interfere with faint world markers and player intel markers.
- Added immersive risk-report generation for nearby towns:
  - infected activity
  - human threat activity
  - medical opportunity
  - intel confidence
  - dynamic field recommendation
- Town intel now evaluates the nearest known capture/settlement location instead of broadcasting multiple nearby markers.
- Added weighted/randomized town risk generation for more varied EoH Hardcore scouting gameplay.
- Kept `EoH_TraderIntel` behavior unchanged.

### Notes
- This is intentionally Phase 1 only.
- Town intel currently produces report text only and does not create markers.
- Future expansion options may include:
  - medical leads
  - local scouting objectives
  - temporary personal search zones
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
