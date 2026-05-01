// Echoes of Humanity - Roaming Trader Intel Bridge
// Phase 1: Trader intel reveal path.
//
// IMPORTANT DESIGN RULE:
// This file does NOT replace the normal "player stumbled across trader" reveal path.
// It is a second/manual reveal path called only when a player consumes trader intel.

class EoH_RT_IntelBridge
{
    // Server-side entry point used by EoH_ActionUseTraderIntel.
    static void RevealTraderFromIntel(PlayerBase player, string intelClassName)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        string playerName = "Unknown Survivor";
        if (player && player.GetIdentity())
            playerName = player.GetIdentity().GetName();

        Print("[EoH Intel] " + playerName + " used trader intel item: " + intelClassName);

        // Keep this intentionally separated from the existing stumble/discovery logic.
        // Wire this to the SAME marker reveal function your trader interaction/discovery code already uses.
        // Do not remove or change your existing on-discovered/on-interact marker call.
        EoH_RT_IntelBridge.CallRoamingTraderReveal(player, intelClassName);
    }

    // Integration shim.
    // Replace the TODO section below with your existing global-marker reveal call once the current
    // EoH_RoamingTrader manager function name is confirmed in the live file.
    static void CallRoamingTraderReveal(PlayerBase player, string intelClassName)
    {
        // TODO IN LIVE INTEGRATION:
        // Call the existing function that reveals the active roaming trader marker globally.
        //
        // Examples only - use whichever function actually exists in your working trader manager:
        // EoH_RT_TraderManager.GetInstance().RevealCurrentTraderMarkerGlobal(player);
        // EoH_RT_TraderManager.RevealTraderMarkerGlobal(player);
        // EoH_RT_MarkerManager.RevealActiveTraderMarkerToAll();
        //
        // The key point: intel should call the same reveal/broadcast function as discovery,
        // but discovery should remain active and unchanged.

        Print("[EoH Intel] TODO: connect CallRoamingTraderReveal() to existing EoH_RoamingTrader global marker reveal function.");
    }
}
