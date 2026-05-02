class EoH_TownMarkerManager
{
    // SERVER: broadcast marker
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

        SendMarkerToAll(data);
    }
};

// CLIENT SIDE HANDLING
modded class PlayerBase
{
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        if (GetGame().IsServer())
            return;

        if (rpc_type == EoH_TownMarkerRPC.ADD_OR_UPDATE_TOWN_MARKER)
        {
            Param1<ref EoH_TownMarkerData> param;
            if (!ctx.Read(param) || !param || !param.param1)
                return;

            EoH_TownMarkerClientManager.Get().SetMarker(param.param1);
            return;
        }

        if (rpc_type == EoH_TownMarkerRPC.REMOVE_TOWN_MARKER)
        {
            Param1<string> param;
            if (!ctx.Read(param) || !param)
                return;

            EoH_TownMarkerClientManager.Get().RemoveMarker(param.param1);
            return;
        }
    }
};

class EoH_TownMarkerClientManager
{
    private static ref EoH_TownMarkerClientManager s_Instance;
    protected ref map<string, ref EoH_TownMarkerData> m_Markers;

    void EoH_TownMarkerClientManager()
    {
        m_Markers = new map<string, ref EoH_TownMarkerData>();
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

    void RemoveMarker(string id)
    {
        ExpansionMarkerModule markerModule;
        CF_Modules<ExpansionMarkerModule>.Get(markerModule);

        if (markerModule)
            markerModule.RemovePersonalMarkerByUID(id);
    }
};
