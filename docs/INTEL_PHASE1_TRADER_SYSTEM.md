# EoH Intel System – Phase 1 (Trader Reveal)

## What this does

Adds a SECOND way to reveal roaming traders:

1. Existing behavior (UNCHANGED):
   - Player finds/interacts with trader → marker reveals globally

2. NEW behavior:
   - Player uses a Trader Intel item → marker reveals globally

## What this does NOT do

- Does NOT remove or modify existing discovery marker logic
- Does NOT interfere with roaming trader movement system

## How to finish integration

Open your live file:

scripts/4_World/eoh_rt_tradermanager.c

Find the function that reveals the trader marker globally.

Then go to:

scripts/4_World/eoh_rt_intel_bridge.c

Replace the TODO inside:

EoH_RT_IntelBridge.CallRoamingTraderReveal()

With your real function call.

## Example (DO NOT blindly copy)

EoH_RT_TraderManager.GetInstance().RevealCurrentTraderMarkerGlobal(player);

## Creating intel items

In your config.cpp:

class EoH_Intel_Trader_Basic : Inventory_Base
{
    scope = 2;
    displayName = "Trader Intel (Basic)";
    descriptionShort = "A scribbled note hinting at a trader's location.";
};

Any item with classname containing:

EoH_Intel_Trader

will automatically get the use action.

## Result

- Discovery path = still works
- Intel path = adds PvP-driven reveal
- Both systems coexist cleanly

## Next step

Phase 2 = Airdrop Intel (player-triggered drops)
