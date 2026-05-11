class EoH_RT_AIIntegration
{
    static void SpawnEscort(EoH_RT_TraderProfile profile, EoH_RT_TraderRuntime runtime, vector centerPos)
    {
        if (!GetGame().IsServer() || !profile || !runtime || !profile.Escort || !profile.Escort.Enabled)
            return;

        CleanupEscort(runtime);

        int count = GetEscortCount(profile);
        eAIFaction faction = GetEscortFaction(profile);
        eAIGroup group = eAIGroup.CreateGroup(faction);

        if (group)
            group.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);

        for (int i = 0; i < count; i++)
        {
            vector pos = GetEscortSpawnPosition(centerPos, i, count);
            Object ai = SpawnExpansionAI(profile, pos, centerPos, group);
            if (ai)
            {
                runtime.EscortUnits.Insert(ai);
                Print("[EoH_RT][ExpansionAI] Spawned trader escort " + ai.GetType() + " trader=" + profile.TraderId + " loadout=" + GetEscortLoadout(profile) + " pos=" + pos.ToString());
            }
            else
            {
                Print("[EoH_RT][ExpansionAI][WARN] Failed to spawn trader escort trader=" + profile.TraderId + " pos=" + pos.ToString());
            }
        }

        if (group)
        {
            group.AddWaypoint(patrolCenterToSurface(centerPos + "8 0 8".ToVector()));
            group.AddWaypoint(patrolCenterToSurface(centerPos + "-8 0 8".ToVector()));
            group.AddWaypoint(patrolCenterToSurface(centerPos + "-8 0 -8".ToVector()));
            group.AddWaypoint(patrolCenterToSurface(centerPos + "8 0 -8".ToVector()));
        }
    }

    static Object SpawnExpansionAI(EoH_RT_TraderProfile profile, vector pos, vector patrolCenter, eAIGroup group)
    {
        pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.1;

        Object obj = GetGame().CreateObjectEx("eAI_SurvivorM_Mirek", pos, ECE_PLACE_ON_SURFACE);
        if (!obj)
            return null;

        eAIBase ai = eAIBase.Cast(obj);
        if (!ai)
            return obj;

        if (group)
            ai.SetGroup(group);

        ai.SetPosition(pos);
        ai.SetOrientation(Vector(Math.RandomFloat(0, 360), 0, 0));
        ApplyEscortSetup(ai, profile);

        return obj;
    }

    static void ApplyEscortSetup(eAIBase ai, EoH_RT_TraderProfile profile)
    {
        if (!ai || !profile)
            return;

        string loadout = GetEscortLoadout(profile);
        if (loadout != "")
            ExpansionHumanLoadout.Apply(ai, loadout, false);

        ai.eAI_SetLootingBehavior(eAILootingBehavior.NONE);
        ai.eAI_SetMovementSpeedLimit(2);
        ai.eAI_SetThreatDistanceLimit(120.0);

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ForcePrimaryToHands, 500, false, ai);
    }

    static void ForcePrimaryToHands(eAIBase ai)
    {
        if (!ai)
            return;

        EntityAI item;
        for (int i = 0; i < ai.GetInventory().AttachmentCount(); i++)
        {
            item = ai.GetInventory().GetAttachmentFromIndex(i);
            if (!item)
                continue;

            if (item.IsWeapon())
            {
                ai.LocalTakeEntityToHands(item);
                return;
            }
        }
    }

    static string GetEscortLoadout(EoH_RT_TraderProfile profile)
    {
        if (!profile)
            return "EoH_AI_HighValue_Hard";

        if (profile.TraderId == "eoh_drug_trader")
            return "EoH_AI_Drug_SMGS";

        if (profile.TraderId == "eoh_black_market")
            return "EoH_AI_HighValue_Hard";

        return "EoH_AI_HighValue_Hard";
    }

    static eAIFaction GetEscortFaction(EoH_RT_TraderProfile profile)
    {
        return new eAIFactionGuards();
    }

    static vector patrolCenterToSurface(vector pos)
    {
        pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.1;
        return pos;
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
