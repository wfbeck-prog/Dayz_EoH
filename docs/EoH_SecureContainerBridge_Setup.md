# EoH Secure Container Bridge Setup

## What this system does

- Gives players a persistent secure container that survives death.
- Container size is controlled by Terje skill level.
- Uses A6 Secure Containers as the item.

## Required mods

- Terje StartScreen
- A6 Secure Containers

## IMPORTANT RULE

Do NOT include secure containers in Terje loadouts.

## Setup Steps

1. Open `scripts/3_Game/eoh_scb_config.c`
2. Replace classnames with your actual A6 items
3. Hook Terje skill system into:

```c
EoH_SCB.GetTerjeSurvivalLevel(player)
```

4. Add logic into MissionServer or PlayerBase:

```c
OnPlayerLoaded:
    delay 10-15 seconds
    determine allowed container tier
    remove invalid containers
    give correct container
```

## Future expansion ideas

- Tie upgrades to Dogtags
- Tie to quests
- Tie to Intel system
- Add blacklisted items
- Add anti-dupe verification

## Notes

This is intentionally separated from other EoH systems so it can later be merged into EoH_Server cleanly.
