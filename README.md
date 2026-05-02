# Dayz_EoH

Echoes of Humanity DayZ server/mod workspace.

This repository is organized around **real DayZ mod roots at the repository root**. Do not place active DayZ mod projects inside a generic `mods/` folder.

## Canonical repository layout

```text
Dayz_EoH/
├── working/
│   ├── EoH_MenuTheme/
│   └── EoH_RoamingTrader/
│
├── EoH_Server/
├── EoH_WorldState/
│
├── mission/
├── docs/
└── archive/
```

## Folder rules

### `working/`
Use this only for **100% working/live backups**.

Files in `working/` are treated as your safe baseline. Do not refactor directly inside `working/` unless the goal is to update the saved known-good copy after testing.

Current working baseline:

- `working/EoH_MenuTheme/`
- `working/EoH_RoamingTrader/`

### Root-level mod folders

Actual DayZ mod projects should live directly at repo root, for example:

- `EoH_Server/`
- `EoH_WorldState/`
- `EoH_MenuTheme/`
- `EoH_RoamingTrader/`

This keeps DayZ Addon Builder, `config.cpp`, `pbo.json`, script paths, and PBO prefixes cleaner.

### `EoH_Server/`
Future unified Echoes of Humanity server mod.

This should become the main home for new EoH gameplay systems over time:

- World state
- Town capture
- Group/clan influence
- AI response systems
- Intel systems
- Event hooks
- Server-side framework code

### `EoH_WorldState/`
Standalone development/test mod for world-state and town-capture systems.

This should not be loaded beside `EoH_Server` long-term if both define the same classes. Once stable, merge useful systems into `EoH_Server` and retire or archive this standalone version.

### `mission/`
Use for mission-side files and server mission references, such as:

- `init.c`
- `mpmissions/dayzOffline.chernarusplus/`
- Expansion settings snapshots

### `docs/`
Use for setup notes, integration plans, classnames, troubleshooting, and system design.

### `archive/`
Use for old, broken, retired, or experimental systems that should not be loaded.

## Important rule

Do **not** use this structure for active mods:

```text
mods/EoH_Server/
mods/EoH_WorldState/
```

That layout is retired for this repo.

## Main goals

- Preserve working live files before refactoring.
- Keep real DayZ mod roots at the repository root.
- Consolidate EoH systems over time into `EoH_Server`.
- Keep experimental systems separate until tested.
- Avoid duplicate class definitions between active mods.
- Track changes safely with GitHub history.
