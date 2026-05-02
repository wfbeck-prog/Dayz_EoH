# EoH Repository Structure (Source of Truth)

This document defines the **official structure** for this repository.

## Core principle

DayZ mods should exist as **top-level folders**, not inside a generic `mods/` directory.

## Correct layout

```text
Dayz_EoH/
├── working/
├── EoH_Server/
├── EoH_WorldState/
├── mission/
├── docs/
└── archive/
```

## Why this matters

- Matches how DayZ tools expect mod roots
- Keeps `config.cpp` paths clean
- Avoids prefix and packing issues
- Prevents confusion when building PBOs

## Working vs Development

- `working/` = stable, live, safe
- `EoH_Server/` = future unified system
- `EoH_WorldState/` = experimental / staging

## Migration rule

When a system is stable:

```text
working/ → copy → EoH_Server/
```

Never refactor directly in `working/`.

## Conflict rule

Do not load multiple mods that define the same classes (example: `EoH_CaptureRelay_Base`).

Merge instead of stacking.

## Status

This file overrides any previous guidance from older chats.
