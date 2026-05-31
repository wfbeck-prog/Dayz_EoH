// Echoes of Humanity - MMG / Gear Nested Storage Patch
// Goal:
// - Allow selected wearable gear to be stored in MMG base storage containers.
// - Allow vests / rigs to be stored in backpacks, even when those vests / rigs are full.
// - Keep SNAFU out of the logic.
//
// Notes:
// - This intentionally does not require the stored vest/rig/backpack to be empty.
// - This can increase effective storage capacity. That is intentional per EoH request.

class EoH_MMG_GearStorageRules
{
    static bool DebugEnabled()
    {
        return false;
    }

    static bool StartsWith(string value, string prefix)
    {
        if (value == "" || prefix == "")
            return false;

        return value.IndexOf(prefix) == 0;
    }

    static bool IsMMGBaseStorage(EntityAI container)
    {
        if (!container)
            return false;

        string type = container.GetType();

        // MMG base storage classnames are all mmg_* in the provided list.
        return StartsWith(type, "mmg_");
    }

    static bool IsBackpackContainer(EntityAI container)
    {
        if (!container)
            return false;

        string type = container.GetType();

        if (container.IsKindOf("Bag_Base"))
            return true;

        if (container.IsInherited(Clothing) && type.Contains("Backpack"))
            return true;

        if (StartsWith(type, "ALV_") && (type.Contains("Bag") || type.Contains("Backpack") || type.Contains("Duffle") || type.Contains("Satchel")))
            return true;

        if (StartsWith(type, "My_DF_Gear_Backpacks_"))
            return true;

        return false;
    }

    static bool IsVestOrRig(EntityAI item)
    {
        if (!item)
            return false;

        string type = item.GetType();

        if (item.IsKindOf("Vest_Base"))
            return true;

        if (item.IsInherited(Clothing) && (type.Contains("Vest") || type.Contains("Rig") || type.Contains("PlateCarrier") || type.Contains("Chest")))
            return true;

        if (StartsWith(type, "ALV_") && (type.Contains("Vest") || type.Contains("Rig") || type.Contains("ChestPouch") || type.Contains("ChestRig")))
            return true;

        if (StartsWith(type, "My_DF_Gear_Armors_"))
            return true;

        if (StartsWith(type, "My_DF_Gear_Rigs_"))
            return true;

        return false;
    }

    static bool IsWearableGear(EntityAI item)
    {
        if (!item)
            return false;

        string type = item.GetType();

        if (item.IsInherited(Clothing))
            return true;

        if (item.IsKindOf("Bag_Base"))
            return true;

        if (item.IsKindOf("Vest_Base"))
            return true;

        if (StartsWith(type, "ALV_"))
            return true;

        if (StartsWith(type, "My_DF_Gear_Armors_"))
            return true;

        if (StartsWith(type, "My_DF_Gear_Backpacks_"))
            return true;

        if (StartsWith(type, "My_DF_Gear_Heads_"))
            return true;

        if (StartsWith(type, "My_DF_Gear_Rigs_"))
            return true;

        return false;
    }

    static bool ShouldForceAllowCargo(EntityAI container, EntityAI item)
    {
        if (!container || !item)
            return false;

        // Main request: full vests / rigs inside backpacks.
        if (IsBackpackContainer(container) && IsVestOrRig(item))
            return true;

        // MMG base storage should accept wearable gear, including modded ALV / Delta gear.
        if (IsMMGBaseStorage(container) && IsWearableGear(item))
            return true;

        return false;
    }
};

modded class ItemBase
{
    override bool CanReceiveItemIntoCargo(EntityAI item)
    {
        if (EoH_MMG_GearStorageRules.ShouldForceAllowCargo(this, item))
        {
            if (EoH_MMG_GearStorageRules.DebugEnabled() && item)
                Print("[EoH_MMG_GearStorage] Force allow cargo: container=" + GetType() + " item=" + item.GetType());

            return true;
        }

        return super.CanReceiveItemIntoCargo(item);
    }

    override bool CanPutInCargo(EntityAI parent)
    {
        if (EoH_MMG_GearStorageRules.ShouldForceAllowCargo(parent, this))
        {
            if (EoH_MMG_GearStorageRules.DebugEnabled() && parent)
                Print("[EoH_MMG_GearStorage] Force allow put-in-cargo: container=" + parent.GetType() + " item=" + GetType());

            return true;
        }

        return super.CanPutInCargo(parent);
    }
};
