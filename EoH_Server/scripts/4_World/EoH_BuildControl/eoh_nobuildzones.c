class EoH_NoBuildZone
{
    string Name;
    int Enabled;
    ref array<float> Center;
    float Radius;
    string DenyMessage;
}

class EoH_NoBuildConfig
{
    int ConfigVersion;
    int Enabled;
    string DefaultDenyMessage;
    ref array<ref EoH_NoBuildZone> Zones;
}

class EoH_NoBuildManager
{
    protected static ref EoH_NoBuildConfig m_Config;
    protected static bool m_Loaded;

    static void Load()
    {
        if (m_Loaded)
            return;

        m_Loaded = true;

        string path = "$profile:EoH_BuildControl/NoBuildZones.json";

        if (!FileExist(path))
        {
            Print("[EoH_NoBuild] Config missing: " + path);
            return;
        }

        m_Config = new EoH_NoBuildConfig();
        JsonFileLoader<EoH_NoBuildConfig>.JsonLoadFile(path, m_Config);

        if (!m_Config)
        {
            Print("[EoH_NoBuild] Failed loading config");
            return;
        }

        int count = 0;
        if (m_Config.Zones)
            count = m_Config.Zones.Count();

        Print("[EoH_NoBuild] Loaded zones: " + count);
    }

    static bool IsBlocked(vector position, out string denyMessage)
    {
        denyMessage = "You cannot build here.";

        if (!m_Loaded)
            Load();

        if (!m_Config)
            return false;

        if (m_Config.Enabled == 0)
            return false;

        if (!m_Config.Zones)
            return false;

        foreach (EoH_NoBuildZone zone : m_Config.Zones)
        {
            if (!zone)
                continue;

            if (zone.Enabled == 0)
                continue;

            if (!zone.Center || zone.Center.Count() < 3)
                continue;

            vector center = Vector(zone.Center[0], zone.Center[1], zone.Center[2]);
            float dist = vector.Distance(position, center);

            if (dist <= zone.Radius)
            {
                if (zone.DenyMessage != string.Empty)
                    denyMessage = zone.DenyMessage;
                else if (m_Config.DefaultDenyMessage != string.Empty)
                    denyMessage = m_Config.DefaultDenyMessage;

                Print("[EoH_NoBuild] Blocked build in zone: " + zone.Name);
                return true;
            }
        }

        return false;
    }
}

modded class Hologram
{
    override bool IsCollidingBBox()
    {
        bool result = super.IsCollidingBBox();

        vector pos = GetProjectionPosition();

        string denyMessage;
        if (EoH_NoBuildManager.IsBlocked(pos, denyMessage))
        {
            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

            if (player && player.GetIdentity())
            {
                Param1<string> msg = new Param1<string>(denyMessage);
                GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, msg, true, player.GetIdentity());
            }

            return true;
        }

        return result;
    }
}
