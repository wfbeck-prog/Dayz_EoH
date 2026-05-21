class EoH_TownAISpawnAdapter
{
    static Object SpawnDebugPlaceholder(string townName, vector pos)
    {
        if (!GetGame() || !GetGame().IsServer())
            return null;

        Object obj = GetGame().CreateObjectEx("WoodenStick", pos, ECE_PLACE_ON_SURFACE);
        if (obj)
        {
            obj.SetPosition(pos);
            obj.PlaceOnSurface();
            Print("[EoH_TownAI][SPAWN][DEBUG] Spawned placeholder for town=" + townName + " pos=" + pos.ToString());
        }
        else
        {
            Print("[EoH_TownAI][SPAWN][WARN] Failed to spawn debug placeholder for town=" + townName + " pos=" + pos.ToString());
        }

        return obj;
    }

    static Object SpawnTownPatrolUnit(string townName, vector pos, vector patrolCenter, string loadout, eAIGroup group)
    {
        if (!GetGame() || !GetGame().IsServer())
            return null;

        pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.1;

        Object obj = GetGame().CreateObjectEx("eAI_SurvivorM_Mirek", pos, ECE_PLACE_ON_SURFACE);
        if (!obj)
        {
            Print("[EoH_TownAI][ExpansionAI][WARN] Failed to spawn patrol unit town=" + townName + " loadout=" + loadout + " pos=" + pos.ToString());
            return null;
        }

        eAIBase ai = eAIBase.Cast(obj);
        if (!ai)
        {
            Print("[EoH_TownAI][ExpansionAI][WARN] Spawned object is not eAIBase town=" + townName + " type=" + obj.GetType());
            return obj;
        }

        ai.EoH_SetTownAITownName(townName);

        if (group)
            ai.SetGroup(group);

        ai.SetPosition(pos);
        ai.SetOrientation(Vector(Math.RandomFloat(0, 360), 0, 0));

        if (loadout != "")
            ExpansionHumanLoadout.Apply(ai, loadout, false);

        ai.eAI_SetLootingBehavior(eAILootingBehavior.NONE);
        ai.eAI_SetThreatDistanceLimit(175.0);

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ForcePrimaryToHands, 500, false, ai);

        Print("[EoH_TownAI][ExpansionAI] Spawned town patrol " + obj.GetType() + " town=" + townName + " loadout=" + loadout + " pos=" + pos.ToString());
        return obj;
    }

    static eAIGroup CreateTownPatrolGroup(vector centerPos)
    {
        eAIFaction faction = new eAIFactionGuards();
        eAIGroup group = eAIGroup.CreateGroup(faction);

        if (group)
        {
            group.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);
            group.AddWaypoint(ToSurface(centerPos + "35 0 35".ToVector()));
            group.AddWaypoint(ToSurface(centerPos + "-35 0 35".ToVector()));
            group.AddWaypoint(ToSurface(centerPos + "-35 0 -35".ToVector()));
            group.AddWaypoint(ToSurface(centerPos + "35 0 -35".ToVector()));
        }

        return group;
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

    static vector ToSurface(vector pos)
    {
        pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.1;
        return pos;
    }
};
