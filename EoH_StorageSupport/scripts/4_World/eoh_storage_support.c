// Echoes of Humanity - Storage Support
// Allows EoH storage containers to accept ALV and My Delta Force wearable gear.
// SNAFU support intentionally removed.

class EoH_StorageSupport
{
    static bool IsAllowedGearStorageItem(EntityAI item)
    {
        if (!item)
            return false;

        string type = item.GetType();

        // Vanilla-compatible wearable gear.
        if (item.IsInherited(Clothing))
            return true;

        if (item.IsKindOf("Bag_Base"))
            return true;

        if (item.IsKindOf("Vest_Base"))
            return true;

        // Alevaric's Clothing Overhaul.
        if (type.IndexOf("ALV_") == 0)
            return true;

        // My Delta Force Gear wearable categories.
        if (type.IndexOf("My_DF_Gear_Armors_") == 0)
            return true;

        if (type.IndexOf("My_DF_Gear_Backpacks_") == 0)
            return true;

        if (type.IndexOf("My_DF_Gear_Heads_") == 0)
            return true;

        if (type.IndexOf("My_DF_Gear_Rigs_") == 0)
            return true;

        return false;
    }

    static bool IsEmptyForNestedStorage(EntityAI item)
    {
        if (!item || !item.GetInventory())
            return true;

        // Prevents backpack-in-backpack / vest-with-loot nesting abuse.
        return item.GetInventory().CountInventory() == 0;
    }

    static bool CanStoreGear(EntityAI item, bool requireEmpty = true, bool debug = false)
    {
        if (!item)
            return false;

        string type = item.GetType();

        if (debug)
            Print("[EoH Storage] Checking item: " + type);

        if (!IsAllowedGearStorageItem(item))
        {
            if (debug)
                Print("[EoH Storage] Rejected class: " + type);

            return false;
        }

        if (requireEmpty && !IsEmptyForNestedStorage(item))
        {
            if (debug)
                Print("[EoH Storage] Rejected non-empty item: " + type);

            return false;
        }

        if (debug)
            Print("[EoH Storage] Accepted item: " + type);

        return true;
    }
}
