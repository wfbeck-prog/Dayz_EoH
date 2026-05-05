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

        if (!m_Flags)
            return false;

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

        return false;
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
