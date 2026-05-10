class EoH_TerritorySpawn
{
    static ref array<Flag_Base> m_Flags;

    static void RegisterFlag(Flag_Base flag)
    {
        if (!m_Flags)
            m_Flags = new array<Flag_Base>();

        if (m_Flags.Find(flag) == -1)
            m_Flags.Insert(flag);
    }

    static void UnregisterFlag(Flag_Base flag)
    {
        if (!m_Flags)
            return;

        m_Flags.RemoveItem(flag);
    }

    static bool GetGroupFlagPosition(PlayerBase player, out vector pos)
    {
        string groupID = EoH_GroupHelper.GetGroupID(player);
        if (groupID == "")
            return false;

        if (m_Flags)
        {
            foreach (Flag_Base flag : m_Flags)
            {
                if (!flag)
                    continue;

                if (flag.GetEoHOwner() == groupID)
                {
                    pos = flag.GetPosition();
                    return true;
                }
            }
        }

        return GetOwnedTownSpawnPosition(player, pos);
    }

    static bool GetOwnedTownSpawnPosition(PlayerBase player, out vector pos)
    {
        if (!player || !player.GetIdentity())
            return false;

        string groupID = EoH_GroupHelper.GetGroupID(player);
        if (groupID == "")
            return false;

        EoH_WorldStateData state = EoH_WorldStateManager.Get().GetState();
        if (!state || !state.Towns)
            return false;

        foreach (EoH_WorldStateTownState town : state.Towns)
        {
            if (!town)
                continue;

            if (town.OwnerGroupID != groupID)
                continue;

            vector townPos = EoH_CaptureManager.Get().GetTownPos(town.Name);
            if (townPos == "0 0 0".ToVector())
                continue;

            pos = GetSafeSpawn(townPos);
            Print("[EoH_TerritorySpawn] Using owned town relay spawn for player=" + player.GetIdentity().GetName() + " town=" + town.Name + " group=" + town.OwnerGroupName + " pos=" + pos.ToString());
            return true;
        }

        return false;
    }

    static bool PlayerOwnsTown(PlayerBase player, string townName)
    {
        if (!player || !player.GetIdentity() || townName == "")
            return false;

        string groupID = EoH_GroupHelper.GetGroupID(player);
        if (groupID == "")
            return false;

        EoH_WorldStateTownState town = EoH_WorldStateManager.Get().GetTownState(townName);
        if (!town)
            return false;

        return town.OwnerGroupID == groupID;
    }

    static vector GetSafeSpawn(vector flagPos)
    {
        vector offset = Vector(Math.RandomFloat(-10,10), 0, Math.RandomFloat(-10,10));
        vector spawn = flagPos + offset;
        spawn[1] = GetGame().SurfaceY(spawn[0], spawn[2]);
        return spawn;
    }
};

modded class Flag_Base
{
    override void EEInit()
    {
        super.EEInit();
        EoH_TerritorySpawn.RegisterFlag(this);
    }

    override void EEDelete(EntityAI parent)
    {
        super.EEDelete(parent);
        EoH_TerritorySpawn.UnregisterFlag(this);
    }
};
