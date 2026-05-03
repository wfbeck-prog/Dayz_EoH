class EoH_TownMarkerManager
{
    static int GetGroupColor(string groupName)
    {
        if (groupName == "")
            return ARGB(255, 200, 200, 200);

        int len = groupName.Length();

        switch (len % 8)
        {
            case 0: return ARGB(255, 255, 80, 80);
            case 1: return ARGB(255, 80, 160, 255);
            case 2: return ARGB(255, 80, 255, 120);
            case 3: return ARGB(255, 255, 200, 80);
            case 4: return ARGB(255, 180, 80, 255);
            case 5: return ARGB(255, 80, 255, 220);
            case 6: return ARGB(255, 255, 120, 200);
            default: return ARGB(255, 200, 200, 200);
        }

        return ARGB(255, 200, 200, 200);
    }

    static void SendMarkerToAll(EoH_TownMarkerData data)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        for (int i = 0; i < players.Count(); i++)
        {
            PlayerBase player = PlayerBase.Cast(players[i]);
            if (!player || !player.GetIdentity())
                continue;

            Param1<ref EoH_TownMarkerData> param = new Param1<ref EoH_TownMarkerData>(data);
            GetGame().RPCSingleParam(player, EoH_TownMarkerRPC.ADD_OR_UPDATE_TOWN_MARKER, param, true, player.GetIdentity());
        }
    }

    static void RemoveMarkerFromAll(string markerId)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        for (int i = 0; i < players.Count(); i++)
        {
            PlayerBase player = PlayerBase.Cast(players[i]);
            if (!player || !player.GetIdentity())
                continue;

            Param1<string> param = new Param1<string>(markerId);
            GetGame().RPCSingleParam(player, EoH_TownMarkerRPC.REMOVE_TOWN_MARKER, param, true, player.GetIdentity());
        }
    }

    static string GetMarkerId(string townName)
    {
        string id = "EoH_TOWN_" + townName;
        id.Replace(" ", "_");
        return id;
    }

    static string GetContestedMarkerId(string townName)
    {
        string id = "EoH_CONTESTED_" + townName;
        id.Replace(" ", "_");
        return id;
    }

    static vector GetTownPosition(string townName)
    {
        EoH_CaptureTownConfig cfg = EoH_CaptureManager.Get().GetTownConfig(townName);
        if (cfg)
            return cfg.GetRelayVector();

        return "0 0 0";
    }

    static void UpdateTownMarker(string townName, string ownerGroupName)
    {
        vector pos = GetTownPosition(townName);
        if (pos == "0 0 0")
            return;

        string id = GetMarkerId(townName);

        EoH_TownMarkerData data = new EoH_TownMarkerData(id, townName, ownerGroupName, pos);
        data.Color = GetGroupColor(ownerGroupName);
        data.BaseColor = data.Color;

        SendMarkerToAll(data);
    }

    static void UpdateContestedMarker(string townName, string attackingGroup)
    {
        vector pos = GetTownPosition(townName);
        if (pos == "0 0 0")
            return;

        string id = GetContestedMarkerId(townName);

        EoH_TownMarkerData data = new EoH_TownMarkerData(id, townName, attackingGroup, pos);
        data.Color = ARGB(255, 255, 50, 50);
        data.BaseColor = data.Color;
        data.IsContested = 1;
        data.Pulse = 1;

        SendMarkerToAll(data);
    }

    static void ClearContestedMarker(string townName)
    {
        RemoveMarkerFromAll(GetContestedMarkerId(townName));
    }
};

modded class PlayerBase
{
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        if (GetGame().IsServer())
            return;

        if (rpc_type == EoH_TownMarkerRPC.ADD_OR_UPDATE_TOWN_MARKER)
        {
            Param1<ref EoH_TownMarkerData> paramAdd;
            if (!ctx.Read(paramAdd) || !paramAdd || !paramAdd.param1)
                return;

            EoH_TownMarkerClientManager.Get().SetMarker(paramAdd.param1);
            return;
        }

        if (rpc_type == EoH_TownMarkerRPC.REMOVE_TOWN_MARKER)
        {
            Param1<string> paramRemove;
            if (!ctx.Read(paramRemove) || !paramRemove)
                return;

            EoH_TownMarkerClientManager.Get().RemoveMarker(paramRemove.param1);
            return;
        }
    }
};

class EoH_TownMarkerClientManager
{
    private static ref EoH_TownMarkerClientManager s_Instance;
    protected ref map<string, ref EoH_TownMarkerData> m_Markers;
    protected float m_PulseTime;

    void EoH_TownMarkerClientManager()
    {
        m_Markers = new map<string, ref EoH_TownMarkerData>();
        m_PulseTime = 0;

        if (!GetGame().IsDedicatedServer())
            GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(OnUpdate);
    }

    void ~EoH_TownMarkerClientManager()
    {
        if (!GetGame().IsDedicatedServer())
            GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Remove(OnUpdate);
    }

    static EoH_TownMarkerClientManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_TownMarkerClientManager();

        return s_Instance;
    }

    void SetMarker(EoH_TownMarkerData data)
    {
        if (!data)
            return;

        m_Markers.Set(data.MarkerId, data);
        ApplyMarker(data);
    }

    void ApplyMarker(EoH_TownMarkerData data)
    {
        ExpansionMarkerModule markerModule;
        CF_Modules<ExpansionMarkerModule>.Get(markerModule);

        if (!markerModule || !markerModule.GetData())
            return;

        ExpansionMarkerData marker = markerModule.GetData().PersonalGetByUID(data.MarkerId);

        if (!marker)
        {
            marker = ExpansionMarkerData.Create(ExpansionMapMarkerType.PERSONAL, data.MarkerId);
            marker.SetIcon(data.Icon);
            markerModule.CreateMarker(marker);
        }

        marker.SetName(data.GetLabel());
        marker.SetPosition(data.Position);
        marker.SetColor(data.Color);

        markerModule.Refresh();
    }

    void OnUpdate(float timeslice)
    {
        m_PulseTime += timeslice;

        foreach (string id, EoH_TownMarkerData data : m_Markers)
        {
            if (!data || data.Pulse != 1)
                continue;

            float wave = Math.AbsFloat(Math.Sin(m_PulseTime * 5.0));

            if (wave > 0.5)
                data.Color = ARGB(255, 255, 255, 255);
            else
                data.Color = data.BaseColor;

            ApplyMarker(data);
        }
    }

    void RemoveMarker(string id)
    {
        if (id == "")
            return;

        m_Markers.Remove(id);

        ExpansionMarkerModule markerModule;
        CF_Modules<ExpansionMarkerModule>.Get(markerModule);

        if (markerModule)
            markerModule.RemovePersonalMarkerByUID(id);
    }
};
