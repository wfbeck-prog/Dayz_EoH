modded class Clothing
{
    bool EoH_IsBackpackContainer(EntityAI container)
    {
        if (!container)
            return false;

        string type = container.GetType();

        if (container.IsKindOf("Bag_Base"))
            return true;

        if (container.IsInherited(Clothing) && type.Contains("Backpack"))
            return true;

        if (type.IndexOf("ALV_") == 0 && (type.Contains("Bag") || type.Contains("Backpack") || type.Contains("Duffle") || type.Contains("Satchel")))
            return true;

        if (type.IndexOf("My_DF_Gear_Backpacks_") == 0)
            return true;

        return false;
    }

    bool EoH_IsVestOrRig()
    {
        string type = GetType();

        if (IsKindOf("Vest_Base"))
            return true;

        if (type.Contains("Vest") || type.Contains("Rig") || type.Contains("PlateCarrier") || type.Contains("Chest"))
            return true;

        if (type.IndexOf("ALV_") == 0 && (type.Contains("Vest") || type.Contains("Rig") || type.Contains("ChestPouch") || type.Contains("ChestRig")))
            return true;

        if (type.IndexOf("My_DF_Gear_Armors_") == 0)
            return true;

        if (type.IndexOf("My_DF_Gear_Rigs_") == 0)
            return true;

        return false;
    }

    override bool CanPutInCargoClothingConditions(EntityAI parent)
    {
        // EoH gameplay rule: allow full vests / rigs to be stored in backpacks.
        // This intentionally bypasses MMG/vanilla empty-clothing checks for this one case.
        if (EoH_IsVestOrRig() && EoH_IsBackpackContainer(parent))
            return true;

        return super.CanPutInCargoClothingConditions(parent);
    }

    override bool CanReceiveItemIntoCargoClothingConditions(EntityAI item)
    {
        // If this clothing item is a backpack, allow it to receive vest/rig cargo even while it is in cargo.
        // MMG blocks clothing-in-cargo from receiving cargo unless the parent is MMG storage; this opens the EoH exception.
        if (EoH_IsBackpackContainer(this) && item && EoH_IsVestOrRigItem(item))
            return true;

        return super.CanReceiveItemIntoCargoClothingConditions(item);
    }

    override bool CanLoadItemIntoCargoClothingConditions(EntityAI item)
    {
        if (EoH_IsBackpackContainer(this) && item && EoH_IsVestOrRigItem(item))
            return true;

        return super.CanLoadItemIntoCargoClothingConditions(item);
    }

    bool EoH_IsVestOrRigItem(EntityAI item)
    {
        if (!item)
            return false;

        string type = item.GetType();

        if (item.IsKindOf("Vest_Base"))
            return true;

        if (item.IsInherited(Clothing) && (type.Contains("Vest") || type.Contains("Rig") || type.Contains("PlateCarrier") || type.Contains("Chest")))
            return true;

        if (type.IndexOf("ALV_") == 0 && (type.Contains("Vest") || type.Contains("Rig") || type.Contains("ChestPouch") || type.Contains("ChestRig")))
            return true;

        if (type.IndexOf("My_DF_Gear_Armors_") == 0)
            return true;

        if (type.IndexOf("My_DF_Gear_Rigs_") == 0)
            return true;

        return false;
    }

    override bool IsContainer()
    {
        if (super.IsContainer())
            return true;

        CargoBase cargo = GetInventory().GetCargo();
        if (!cargo)
            return false;

        if (cargo.GetItemCount() <= 0)
            return false;

        EntityAI parent = GetHierarchyParent();
        if (!parent)
            return true;

        if (parent.IsMan())
            return false;

        string parentType = parent.GetType();
        if (parentType.Contains("Zombie") || parentType.Contains("Zmb"))
            return false;

        if (parentType.Contains("UndergroundStash"))
            return false;

        if (parentType.Contains("Smersh") || GetType().Contains("Smersh"))
            return false;

        return true;
    }
}
