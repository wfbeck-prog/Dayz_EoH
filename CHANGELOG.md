# Changelog

All notable EoH_Server repository changes are tracked here.

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

### Changed
- Changed EoH loot-room direction from CBD/BS room tracking to DNA Keycard Crates / Loot Cache Intel.
- Renamed player-facing loot-room intel wording toward `Loot Cache Intel`.
- Updated intel item config inheritance so EoH intel documents inherit from `ItemBook` via `EoH_Intel_Document`.
- Updated roaming trader AI integration to use proper EoH guard loadouts:
  - Drug trader guards use `EoH_AI_Drug_SMGS`
  - Black market guards use `EoH_AI_HighValue_Hard`
- Updated roaming trader AI guard faction/group handling to reduce friendly-fire/self-kill issues.
- Updated roaming trader relocation marker cleanup to aggressively remove possible stale marker IDs.
- Updated trader intel Discord wording:
  - success now says `Hidden roaming trader signal was found.`
  - unavailable now says `No unrevealed roaming trader signal was available.`
- Updated marker helper fallback logic to use the newly added `CACHE` category and `ACTIVE` state instead of temporary `TRADER` / `NORMAL` fallbacks.
- Updated server status webhook route to use the main `EoH_DiscordWebhook.SendServerStatus(...)` path.
- Updated kill feed logic to skip AI-vs-AI deaths.
- Updated high-value kill detection for long-range and military-zone engagements.
- Updated Delta Force durability balancing:
  - backpacks increased from 500 HP baseline to 875 HP
  - rigs/headgear increased from 100 HP baseline to 200 HP
  - light hat handling increased from 50 HP baseline to 100 HP
  - armor protection, cargo, slots, weight, and inventory sizes were intentionally not changed.

### Removed / Deprecated
- Deprecated active CBD LootRooms integration for EoH progression.
- Deprecated BS KeyRoom remnants for EoH progression.
- Deprecated CJ187/JD LootChests as a possible future loot-progression dependency if DNA crate testing validates.
- Removed the unsupported Expansion AI call `eAI_SetMovementSpeedLimit(...)` from roaming trader AI setup.
- Removed/avoided BS key and CBD key entries from converted DNA loot pool direction.

### Fixed
- Fixed `EoH_MarkerCategory.LOOT` compile error by adding a proper `CACHE` marker category and using it for DNA cache markers.
- Fixed `EoH_MarkerState.ACTIVE` compile error by adding a proper `ACTIVE` marker state.
- Fixed `eAIBase.eAI_SetMovementSpeedLimit` compile error by removing the unsupported method call.
- Fixed multiline Red Ledger webhook call syntax in `eoh_killfeed.c`.
- Fixed webhook config generation so new Dogtag, Server Status, and High Value Kill fields appear in `EoH_Webhooks.json`.
- Fixed intel item inheritance warning/spam by using `ItemBook` as the stable parent for intel documents.
- Fixed server status webhook not firing by adding a delayed MissionServer startup hook.

### Notes
- CBD and BS KeyRoom should remain removed from the active server mod stack once DNA testing validates crate locking, loot spawning, persistence, and reset behavior.
- Purple DNA crates are now planned for bunker/special-cache use rather than being removed completely.
- DNA crate-open marker support is ready, but still needs the exact DNA crate open callback/action hook confirmed during testing.
- `EoH_DNACacheOpenBridge.OnCrateOpened(crate)` is the intended hook target once the DNA open action is identified.
- Server owners must enable and populate the desired webhook URLs in `$profile:EoH\\Webhooks\\EoH_Webhooks.json`.

## 2026-05-06

### Added
- Added Terje StartScreen/EoH territory respawn foundation:
  - `EoH_TerritorySpawnBridge.c`
  - `EoH_TerjeTerritoryRespawnHandler.c`
  - `EoH_TerjeTownRespawnHandler.c`
- Added support for owned territory flag respawns through Terje respawn objects.
- Added support for controlled town relay respawns through `EoH_RadioRelay` as the physical Terje respawn object.
- Added `EoH_Server/config/Terje/EoH_TerjeRespawns_Example.xml` with example Terje XML blocks for:
  - `eoh_town_relay`
  - `eoh_territory_flag`
- Added controlled-town spawn export support to `EoH_TerritorySpawnBridge`.
- Added `EoH_TerjeRespawnCooldowns.c` helper, but Terje native `<Timeout>` is now the preferred cooldown method.

### Changed
- Changed EoH Terje town respawn design to use the existing `EoH_RadioRelay` object instead of a separate town terminal object.
- Updated Terje respawn design so cooldowns are configured in Terje XML using native `<Timeout id="..." minutes="..." />` conditions.
- Updated relay activation broadcasts to use group names via `EoH_GroupHelper.GetGroupName()` instead of individual player names.
- Changed relay auto-spawn config default to opt-in by setting `SpawnRelaysOnServerStart = false` in `Relays.json` generation.
- Updated intel item script inheritance to avoid direct `BookDeadSouls` script dependency.
- Updated intel item actions so `EoH_TownIntel` and `EoH_TraderIntel` attach `EoH_ActionReadIntel` directly.
- Updated BuildControl placement validation so `TerritoryFlagKit` skips expensive world scans during hologram preview.

### Fixed
- Fixed severe lag/stutter while preview-placing TerritoryFlagKit by removing repeated large-radius object scans during placement preview.
- Fixed `eoh_intel_item.c(19): Unknown type 'BookDeadSouls'` by making trader intel inherit from the stable EoH intel document script base.
- Fixed intel item action registration after switching to concrete vanilla/admin-visible item classes.
- Fixed EoH item stability by moving physical relay/intel items toward vanilla-safe inventory bases.
- Fixed relay/town respawn validation so players cannot use respawn points unless their group controls the town or owns the flag.

### Notes
- Terje XML should use `handler="EoH_TerjeTownRespawnHandler"` for `EoH_RadioRelay` town relay respawns.
- Terje XML should use `handler="EoH_TerjeTerritoryRespawnHandler"` for owned `TerritoryFlag` respawns.
- Terje native `<Timeout>` should control cooldowns instead of custom EoH cooldown attributes.
- Relay auto-spawn remains disabled by default until physical relay/item stability and placement behavior are fully validated.

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
- Added `EoH_TerritoryOwnershipRegistry.c` as the replacement ownership backend after `TerritoryFlag` and `Flag_Base` proved unrelated in the active mod stack.
- Added persistent territory ownership storage at `$profile:EoH_Server/Territories.json`.
- Added RoamingTrader files into `EoH_Server` so the server can move toward one consolidated mod.
- Added RoamingTrader PlayerBase proximity reveal hook so finding a trader can reveal its marker.
- Added `EoH_ActionReadIntel` to support reading/consuming EoH intel items.
- Added trader-intel reveal support through `EoH_RT_TraderManager.RevealNearestHiddenTraderToPlayer()`.

### Changed
- Merged RoamingTrader config definitions into `EoH_Server/config.cpp`.
- Restored RoamingTrader Expansion Market class usage:
  - drug trader uses `ExpansionTraderChem`
  - black market trader uses `ExpansionTraderBroker`
- Expanded `EoH_CaptureManager.InitTowns()` from 4 towns to the full EoH capture/town marker set.
- Converted town marker manager into a wrapper around the unified marker service.
- Converted intel marker reveal to use `EoH_MarkerService` instead of old raw marker/RPC logic.
- Split intel reveal behavior:
  - `EoH_TownIntel` / `EoH_Intel_Document` reveals nearby town intel to the reader.
  - `EoH_TraderIntel` reveals the nearest hidden roaming trader to the reader.
- Converted MissionServer base town markers to use unified marker data instead of old raw RPC `777001`.
- Updated MissionServer town marker icon from invalid `Flag` to Expansion-valid `Territory`.
- Wired `MissionServer` tick loop to update both capture manager and RoamingTrader manager.
- Forced `BuildControl.json` generation/load during `MissionServer` initialization by calling `GetEoHBuildControlConfig()` on startup.
- Added `GetTownOwner()` wrapper to `EoH_CaptureManager`, reading persistent ownership from `EoH_WorldStateManager`.
- Updated BuildControl ownership checks to use the EoH ownership registry direction instead of relying on `TerritoryFlag`/`Flag_Base` inheritance.
- Updated `TerritoryFlagKit` placement flow to find the placed flag near the kit position instead of using invalid network ID methods or spawning a duplicate flag.
- Updated flag ownership assignment to store owner group ID in `EoH_TerritoryOwnershipRegistry`.
- Hardened `EoH_GroupHelper`:
  - uses `Expansion_GetPartyID()` first
  - falls back to `Expansion_GetParty()`
  - falls back to solo ownership using player identity
- Changed RoamingTrader AI integration so EoH_Server no longer spawns raw/naked AI. Trader/town AI is now intended to be maintained through `DynamicAI.json` / the EoH DynamicAIB template.
- Updated intel/radio physical item config to use safer vanilla base/model combinations:
  - intel uses `Paper` with `\dz\gear\consumables\Paper.p3d`
  - radio relay uses `Transmitter_Base` with `\dz\gear\radio\WalkieTalkie.p3d`
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
- Removed invalid direct `Flag_Base` cast usage from `TerritoryFlagKit` after compiler showed `TerritoryFlag` and `Flag_Base` are not related.

### Fixed
- Fixed multiple DayZ compile blockers during EoH_Server consolidation:
  - duplicate class declarations
  - invalid action overrides
  - invalid raw RPC marker handlers
  - stale references to deleted marker classes
  - `TerritoryFlag.SetEoHOwner` / `GetEoHOwner` undefined calls
  - invalid `Flag_Base` / `TerritoryFlag` cast path
  - `TerritoryFlagKit.GetNetworkIDLow` / `GetNetworkIDHigh` undefined calls
  - `EoH_IntelManager.ResetPlayerIntelUsage` missing
  - `EoH_IntelManager.RevealTownIntel` missing
  - `EoH_CaptureManager.GetTownOwner` missing
  - `EoH_RT_TraderManager.RevealNearestHiddenTraderToPlayer` missing
  - MissionServer multi-line RPC syntax failure
- Fixed RoamingTrader reveal architecture so each trader reveal is tracked independently by trader ID.
- Fixed intel manager to reset player intel usage on death and send per-player intel markers through the unified marker system.
- Fixed flag placement group detection so Expansion groups and solo players both work.
- Fixed flag auto-build path to use the actual placed flag found near the placement location.
- Fixed BuildControl config generation not running until placement checks by forcing load/generation on server init.
- Fixed town markers only showing 4 entries by expanding the town registry.
- Fixed invalid Expansion marker icons causing white/square placeholders by moving to known valid names.
- Fixed no-model/crash-prone intel item config by switching to a safe vanilla paper model.
- Fixed no-model radio relay config by switching to the vanilla walkie-talkie model.

### Known follow-ups
- Verify admin-spawned `EoH_TownIntel`, `EoH_TraderIntel`, and `EoH_RadioRelay` after rebuilding PBO.
- Verify RoamingTrader marker reveal after rebuilding PBO and approaching each trader.
- Verify `Territories.json` persists territory ownership across server restart.
- Verify `BuildControl.json` generates under `$profile:EoH_Server/BuildControl.json` after server init.
- Rebuild proper Terje territory respawn integration using real Terje StartScreen classes from uploaded reference files.
- Decide whether bunker punchcard event should hook via exact `ActionUseUndergroundPanel` method from the installed Expansion/DayZ version.
- Maintain live AIB guards manually in server `DynamicAI.json` using `DynamicAIB_EoH_Template.json` as the source template.
- Fix live `$profile:\AI_Bandits\DynamicAIB.json` JSON syntax before relying on AI_Bandits runtime features; current crash logs show malformed JSON/null config values.
