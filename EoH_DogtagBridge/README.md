# EoH_DogtagBridge

Root-level DayZ bridge mod for Echoes of Humanity.

This bridge connects:

- **Dogtags** by Wardog
- **Terje Skills**

## Purpose

Dogtags already supports replacing a player's attached tag through:

```c
PlayerBase.ReplaceDogtag(string dogtag_type)
```

Terje Skills exposes skill data through:

```c
player.GetTerjeSkills().GetSkillLevel(skillId)
```

This bridge reads Terje skill levels and upgrades the player's active dogtag variant.

## Default EoH tier rules

The bridge calculates a composite score from several Terje skills:

- `hunt`
- `surv`
- `athlc`
- `stlh`
- `mtblsm`
- `fish`

The highest available score determines the dogtag class:

| Score | Dogtag |
| --- | --- |
| 0-24 | `Dogtag_Survivor` |
| 25-49 | `Dogtag_Silver` |
| 50-74 | `Dogtag_Granite` |
| 75-99 | `Dogtag_Cobalt` |
| 100+ | `Dogtag_Hero` |

## Why no `Dogtag_Bandit` by default?

The Dogtags mod includes `Dogtag_Bandit`, but Terje skill progression does not tell us morality, murder count, or faction behavior by itself. For EoH, this bridge treats Terje progression as survivor value, not hero/bandit morality.

`Dogtag_Bandit` should be added later from a PvP/bounty/karma system.

## Runtime behavior

- Dogtags are evaluated when the Dogtags mod equips a player.
- Existing connected players are periodically re-evaluated.
- This allows dogtags to progress as Terje skills improve.

## Load order

Recommended:

```text
@CF;@Terje-Core;@Terje-Skills;@Dogtags;@EoH_DogtagBridge
```

The bridge should load after both Dogtags and Terje Skills.
