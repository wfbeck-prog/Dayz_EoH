# EoH_DogtagBridge

Bridge concept for tying Terje skill progression into Dogtags-style PvP progression for Echoes of Humanity.

## Goal

Make dogtags matter beyond simple PvP kills:

- Higher-skilled players can drop higher-value tags.
- Dogtag value can feed black market, intel, bounty, or bunker-adjacent systems.
- The bridge stays isolated so it can later be merged into `EoH_Server`.

## Current implementation status

This first commit is a safe scaffold. It does **not** assume the exact Terje or Dogtags internal API names yet.

Included:

- Config-driven tier definitions.
- Terje adapter placeholder.
- Dogtag tier resolver.
- Server-side bridge manager.
- Integration notes for the next pass.

## Planned flow

1. Player dies.
2. Bridge reads the dead player's Terje combat/survival/medical/etc. skill levels.
3. Bridge resolves the correct EoH dogtag tier.
4. Bridge either:
   - replaces the dropped dogtag class, or
   - stores metadata/value for later trader/intel use.

## Recommended next pass

Before enabling runtime hooks, confirm the actual class/function names from:

- Terje skill mod scripts/API
- Dogtags mod scripts/API

Then update:

- `scripts/4_World/eoh_dt_terje_adapter.c`
- `scripts/4_World/eoh_dt_dogtag_adapter.c`
- optional PlayerBase death hook file
