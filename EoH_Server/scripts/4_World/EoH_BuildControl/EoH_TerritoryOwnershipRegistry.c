class EoH_TerritoryOwnershipRegistry
{
    protected static ref map<string, string> s_FlagOwners;

    static void Ensure()
    {
        if (!s_FlagOwners)
            s_FlagOwners = new map<string, string>();
    }

    static void SetOwnerAtPosition(vector pos, string ownerGroupID)
    {
        Ensure();
        s_FlagOwners.Set(MakePositionKey(pos), ownerGroupID);
    }

    static string GetOwnerAtPosition(vector pos)
    {
        Ensure();
        string key = MakePositionKey(pos);
        if (s_FlagOwners.Contains(key))
            return s_FlagOwners.Get(key);

        return "";
    }

    static string GetOwnerForObject(Object obj)
    {
        if (!obj)
            return "";

        return GetOwnerAtPosition(obj.GetPosition());
    }

    static string MakePositionKey(vector pos)
    {
        int x = Math.Round(pos[0]);
        int z = Math.Round(pos[2]);
        return x.ToString() + "_" + z.ToString();
    }
}
