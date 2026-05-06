class EoH_TerritorySpawnPoint
{
    string Name;
    string OwnerGroupID;
    ref array<float> Position;
    float Radius;
    bool Enabled;

    void EoH_TerritorySpawnPoint()
    {
        Name = "";
        OwnerGroupID = "";
        Position = new array<float>();
        Radius = 30.0;
        Enabled = true;
    }

    vector GetPosition()
    {
        if (!Position || Position.Count() < 3)
            return "0 0 0".ToVector();

        return Vector(Position.Get(0), Position.Get(1), Position.Get(2));
    }

    void SetPosition(vector pos)
    {
        Position.Clear();
        Position.Insert(pos[0]);
        Position.Insert(pos[1]);
        Position.Insert(pos[2]);
    }
}

class EoH_TerritorySpawnBridge
{
    static ref array<ref EoH_TerritorySpawnPoint> GetSpawnPointsForPlayer(PlayerBase player)
    {
        array<ref EoH_TerritorySpawnPoint> result = new array<ref EoH_TerritorySpawnPoint>();

        if (!player)
            return result;

        string groupID = EoH_GroupHelper.GetGroupID(player);
        if (groupID == "")
            return result;

        array<ref EoH_TerritoryOwnershipEntry> entries = EoH_TerritoryOwnershipRegistry.GetAllTerritories();
        foreach (EoH_TerritoryOwnershipEntry entry : entries)
        {
            if (!entry || entry.OwnerGroupID != groupID)
                continue;

            EoH_TerritorySpawnPoint sp = new EoH_TerritorySpawnPoint();
            sp.Name = "Territory Flag";
            sp.OwnerGroupID = entry.OwnerGroupID;
            sp.SetPosition(SafeSpawnPosition(entry.GetPosition()));
            sp.Radius = 30.0;
            sp.Enabled = true;
            result.Insert(sp);
        }

        return result;
    }

    static vector SafeSpawnPosition(vector flagPos)
    {
        vector pos = flagPos;
        pos[0] = pos[0] + 3.0;
        pos[2] = pos[2] + 3.0;
        pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.2;
        return pos;
    }
}
