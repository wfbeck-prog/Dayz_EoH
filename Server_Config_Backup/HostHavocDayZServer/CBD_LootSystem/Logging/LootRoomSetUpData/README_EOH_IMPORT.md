# EoH CBD LootRoomSetUpData Import Notes

This folder contains CBD admin/setup exports. Each subfolder is expected to contain one JSON file.

## GitHub web UI issue

GitHub's web UI can show folders, but the ChatGPT GitHub connector can only fetch exact file paths. It cannot recursively browse this folder from a `tree` URL.

## Best way to give ChatGPT the room list from the GitHub web UI

1. Open this folder in GitHub:

```text
Server_Config_Backup/HostHavocDayZServer/CBD_LootSystem/Logging/LootRoomSetUpData
```

2. Click each room subfolder.
3. Click the single JSON file inside.
4. Copy the full repository path from the file page.
5. Paste the paths into ChatGPT.

Example path format:

```text
Server_Config_Backup/HostHavocDayZServer/CBD_LootSystem/Logging/LootRoomSetUpData/<RoomFolder>/<RoomFile>.json
```

## Preferred alternative

Download the `LootRoomSetUpData` folder or repo as a ZIP and upload it directly to ChatGPT. That allows batch processing of all room exports.

## EoH target conversion

The intended final output should be:

```text
Server_Config_Backup/HostHavocDayZServer/CBD_LootSystem/Settings/LootSystemSettings.json
Server_Config_Backup/HostHavocDayZServer/CBD_LootSystem/Settings/LootTables.json
Server_Config_Backup/HostHavocDayZServer/CBD_LootSystem/EoH_CBD_ROOM_MANIFEST.md
```

## Tier plan

- T1 rooms: red key, low/mid loot, chance for T2 key
- T2 rooms: yellow key, mid loot/drug economy, chance for T3 key
- T3 rooms: advanced key, high but not epic loot, chance for T4 key
- T4 rooms: black/lab keycard, bunker prep and punchcard
- Bunker: epic/endgame only
