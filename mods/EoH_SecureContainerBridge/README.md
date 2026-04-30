# EoH_SecureContainerBridge

Bridge mod concept for Echoes of Humanity.

Purpose:

- Keep Terje StartScreen for spawn location and post-death point loadouts.
- Keep A6 Secure Containers as the actual secure container item provider.
- Add EoH logic to save/restore one account-bound secure container after death.
- Gate the allowed secure container tier by Terje skill progression.

Current status: scaffold / integration-ready.

Important:

- Do **not** add A6 secure containers to Terje StartScreen loadout items.
- This bridge should run after Terje finishes applying the selected respawn kit.
- Exact A6 container classnames must be filled in inside `scripts/3_Game/eoh_scb_config.c`.
- Exact Terje skill getter must be wired inside `EoH_SCB_GetTerjeSurvivalLevel` once verified against your installed Terje API.

Suggested progression:

```text
Survival level 0-2 -> no secure container
Survival level 3   -> 2x2 A6 secure container
Survival level 5   -> 3x3 A6 secure container
Survival level 8   -> 4x4 A6 secure container max
```

Save path target:

```text
$profile:EoH/SecureContainers/<Steam64ID>.json
```
