modded class Clothing
{
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
