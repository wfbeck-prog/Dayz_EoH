class EoH_RT_AIIntegration
{
    static void SpawnEscort(EoH_RT_TraderProfile profile, EoH_RT_TraderRuntime runtime, vector centerPos)
    {
        if (!GetGame().IsServer() || !profile || !runtime || !profile.Escort || !profile.Escort.Enabled)
            return;

        CleanupEscort(runtime);

        int count = GetEscortCount(profile);
        string loadout = GetExpansionLoadout(profile);
        string faction = GetExpansionFaction(profile);

        for (int i = 0; i < count; i++)
        {
            vector pos = GetEscortSpawnPosition(centerPos, i, count);
            Object ai = SpawnExpansionAI(pos, centerPos, loadout, faction);
            if (ai)
            {
                runtime.EscortUnits.Insert(ai);
                Print("[EoH_RT][ExpansionAI] Spawned trader escort " + ai.GetType() + " trader=" + profile.TraderId + " loadout=" + loadout + " pos=" + pos.ToString());
            }
            else
            {
                Print("[EoH_RT][ExpansionAI][WARN] Failed to spawn trader escort trader=" + profile.TraderId + " loadout=" + loadout + " pos=" + pos.ToString());
            }
        }
    }

    static Object SpawnExpansionAI(vector pos, vector patrolCenter, string loadout, string faction)
    {
        pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.1;

        Object obj = GetGame().CreateObjectEx("eAI_SurvivorM_Mirek", pos, ECE_PLACE_ON_SURFACE);
        if (!obj)
            return null;

        eAIBase ai = eAIBase.Cast(obj);
        if (!ai)
            return obj;

        ai.SetPosition(pos);
        ai.SetOrientation(Vector(Math.RandomFloat(0, 360), 0, 0));

        if (loadout != "")
            ai.SetLoadout(loadout);

        if (faction != "")
            ai.SetFactionTypeID(faction);

        ai.GetGroup().SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);
        ai.GetGroup().AddWaypoint(patrolCenter + "8 0 8".ToVector(), 8.0);
        ai.GetGroup().AddWaypoint(patrolCenter + "-8 0 8".ToVector(), 8.0);
        ai.GetGroup().AddWaypoint(patrolCenter + "-8 0 -8".ToVector(), 8.0);
        ai.GetGroup().AddWaypoint(patrolCenter + "8 0 -8".ToVector(), 8.0);

        return obj;
    }

    static vector GetEscortSpawnPosition(vector centerPos, int index, int count)
    {
        float radius = 8.0;
        if (count > 4)
            radius = 12.0;

        float angle = (360.0 / Math.Max(count, 1)) * index;
        float rad = angle * Math.DEG2RAD;

        vector pos = centerPos;
        pos[0] = centerPos[0] + Math.Cos(rad) * radius;
        pos[2] = centerPos[2] + Math.Sin(rad) * radius;
        pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.1;
        return pos;
    }

    static int GetEscortCount(EoH_RT_TraderProfile profile)
    {
        if (!profile)
            return 4;

        if (profile.TraderId == "eoh_black_market")
            return 6;

        if (profile.TraderId == "eoh_drug_trader")
            return 4;

        return 3;
    }

    static string GetExpansionLoadout(EoH_RT_TraderProfile profile)
    {
        if (!profile)
            return "EoH_BlackMarketEscort";

        if (profile.TraderId == "eoh_drug_trader")
            return "EoH_DrugEscort";

        if (profile.TraderId == "eoh_black_market")
            return "EoH_BlackMarketEscort";

        if (profile.Escort && profile.Escort.AIGroupType != "")
            return profile.Escort.AIGroupType;

        return "EoH_BlackMarketEscort";
    }

    static string GetExpansionFaction(EoH_RT_TraderProfile profile)
    {
        if (!profile)
            return "Guards";

        if (profile.TraderId == "eoh_black_market")
            return "Mercenaries";

        if (profile.TraderId == "eoh_drug_trader")
            return "Raiders";

        return "Guards";
    }

    static void CleanupEscort(EoH_RT_TraderRuntime runtime)
    {
        if (!runtime || !runtime.EscortUnits)
            return;

        foreach (Object obj : runtime.EscortUnits)
        {
            if (obj)
            {
                Print("[EoH_RT][ExpansionAI] Removing trader escort " + obj.GetType() + " at " + obj.GetPosition().ToString());
                GetGame().ObjectDelete(obj);
            }
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
