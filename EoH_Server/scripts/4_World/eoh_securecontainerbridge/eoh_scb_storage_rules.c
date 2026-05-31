// Echoes of Humanity - Secure Container Storage Rules
// Purpose: allow EoH secure containers to accept selected wearable gear from
// Alevaric's Clothing Overhaul and My Delta Force Gear without relying on SNAFU.

class EoH_SCB_StorageRules
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

        // My Delta Force Gear wearable categories only.
        // Do not include broad My_DF_Gear_Items_ here; those are utility items, not wearable gear.
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

        // Blocks storing loaded backpacks, vests, rigs, or clothing with cargo/attachments.
        return item.GetInventory().CountInventory() == 0;
    }

    static bool CanStoreGear(EntityAI item, bool requireEmpty = true, bool debug = false)
    {
        if (!item)
            return false;

        string type = item.GetType();

        if (debug)
            Print("[EoH_SCB][StorageRules] Checking item: " + type);

        if (!IsAllowedGearStorageItem(item))
        {
            if (debug)
                Print("[EoH_SCB][StorageRules] Rejected class: " + type);

            return false;
        }

        if (requireEmpty && !IsEmptyForNestedStorage(item))
        {
            if (debug)
                Print("[EoH_SCB][StorageRules] Rejected non-empty item: " + type);

            return false;
        }

        if (debug)
            Print("[EoH_SCB][StorageRules] Accepted item: " + type);

        return true;
    }
};
