class EoH_RT_AIIntegration
{
    static void SpawnEscort(EoH_RT_TraderProfile profile, EoH_RT_TraderRuntime runtime, vector centerPos)
    {
        if (!GetGame().IsServer() || !profile || !runtime || !profile.Escort || !profile.Escort.Enabled)
            return;

        string groupName = GetDynamicAIBGroupName(profile);
        Print("[EoH_RT][AIB] Trader escort is managed by DynamicAIB group: " + groupName + " near " + centerPos.ToString());
        Print("[EoH_RT][AIB] No raw trader AI spawned by EoH_Server. Maintain guards in DynamicAI.json / DynamicAIB_EoH_Template.json.");
    }

    static void CleanupEscort(EoH_RT_TraderRuntime runtime)
    {
        if (!runtime || !runtime.EscortUnits)
            return;

        // DynamicAIB owns trader/town guard lifecycle now. EoH_Server only clears any legacy script-spawned units.
        foreach (Object obj : runtime.EscortUnits)
        {
            if (obj)
                GetGame().ObjectDelete(obj);
        }

        runtime.EscortUnits.Clear();
    }

    static string GetDynamicAIBGroupName(EoH_RT_TraderProfile profile)
    {
        if (!profile)
            return "EoH_TRADER_BlackMarketGuards";

        if (profile.TraderId == "eoh_drug_trader")
            return "EoH_TRADER_DrugTraderGuards";

        if (profile.TraderId == "eoh_black_market")
            return "EoH_TRADER_BlackMarketGuards";

        if (profile.Escort && profile.Escort.AIGroupType != "")
            return profile.Escort.AIGroupType;

        return "EoH_TRADER_BlackMarketGuards";
    }
};
