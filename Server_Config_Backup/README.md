# EoH Server Config Backup

This folder is the canonical backup area for custom server-side configuration needed to rebuild the Echoes of Humanity DayZ server from scratch.

## Purpose

Use this folder to store clean, known-good copies of all custom configs that live outside the compiled `EoH_Server` PBO or outside Steam Workshop mods.

This should include:

- mission files
- profile configs
- Expansion configs
- trader economy files
- Terje StartScreen/Skills configs
- AI_Bandits / DynamicAI configs
- event and loot economy XML
- custom types/spawnabletypes/cfgspawnabletypes
- deployment notes
- server launch/load-order notes

## Rules

1. Do not store private passwords, Steam tokens, RCON passwords, database credentials, or host panel credentials here.
2. Prefer sanitized examples when a file contains secrets.
3. Keep configs organized by system/mod.
4. When a live server config is changed, copy the final working version here.
5. Add notes explaining where the file belongs on the server.

## Suggested rebuild order

1. Install DayZ server files.
2. Install workshop mods in the documented load order.
3. Copy mission files from `Mission/`.
4. Copy profile configs from `Profiles/`.
5. Copy Expansion configs from `Expansion/`.
6. Copy economy/XML files from `Economy/`.
7. Copy Terje configs from `Terje/`.
8. Copy AI configs from `AI/`.
9. Copy EoH generated/server configs from `EoH_Server_Profile/`.
10. Review `Deployment/` notes before first boot.
