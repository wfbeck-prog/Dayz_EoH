class EoH_TerjeOwnedRespawnEntry
{
    string Id;
    string DisplayName;
    string Type;
    string OwnerGroupID;
    string OwnerGroupName;
    string TownName;
    vector Position;

    void EoH_TerjeOwnedRespawnEntry()
    {
        Id = "";
        DisplayName = "";
        Type = "";
        OwnerGroupID = "";
        OwnerGroupName = "";
        TownName = "";
        Position = "0 0 0".ToVector();
    }
}

class EoH_TerjeOwnedRespawnRegistry
{
    static ref array<ref EoH_TerjeOwnedRespawnEntry> BuildOwnedRespawns(PlayerBase player)
    {
        ref array<ref EoH_TerjeOwnedRespawnEntry> results = new array<ref EoH_TerjeOwnedRespawnEntry>();

        if (!player || !player.GetIdentity())
            return results;

        string playerGroupID = EoH_GroupHelper.GetGroupID(player);
        if (playerGroupID == "")
            return results;

        AddOwnedTownRespawns(player, playerGroupID, results);
        // Territory scanning is intentionally disabled for now. A 30km GetObjectsAtPosition3D scan during
        // Start Screen generation causes slow respawn-screen loads. We will replace this with a cached
        // territory registry later.
        // AddOwnedTerritoryRespawns(player, playerGroupID, results);

        Print("[EoH_TerjeRespawn][REGISTRY] player=" + player.GetIdentity().GetName() + " groupId=" + playerGroupID + " ownedRespawns=" + results.Count().ToString());
        return results;
    }

    static ref array<string> GetKnownTownNames()
    {
        ref array<string> towns = new array<string>();
        towns.Insert("Pustoshka");
        towns.Insert("Mogilevka");
        towns.Insert("Guglovo");
        towns.Insert("Tulga");
        towns.Insert("Nadezhdino");
        towns.Insert("Kamenka");
        towns.Insert("Vybor");
        towns.Insert("Stary Sobor");
        towns.Insert("Novy Sobor");
        towns.Insert("Zelenogorsk");
        towns.Insert("Staroye");
        towns.Insert("Polana");
        towns.Insert("Elektro");
        towns.Insert("Chernogorsk");
        towns.Insert("Berezino");
        towns.Insert("NWAF");
        towns.Insert("Tisy");
        towns.Insert("Pavlovo Military");
        return towns;
    }

    static void AddOwnedTownRespawns(PlayerBase player, string playerGroupID, array<ref EoH_TerjeOwnedRespawnEntry> results)
    {
        if (!results || playerGroupID == "")
            return;

        ref array<string> townNames = GetKnownTownNames();
        foreach (string townName : townNames)
        {
            EoH_WorldStateTownState state = EoH_WorldStateManager.Get().GetTownState(townName);
            if (!state || state.OwnerGroupID == "" || state.OwnerGroupID != playerGroupID)
                continue;

            vector pos = EoH_CaptureManager.Get().GetTownPos(townName);
            if (pos == "0 0 0".ToVector())
                continue;

            EoH_TerjeOwnedRespawnEntry entry = new EoH_TerjeOwnedRespawnEntry();
            entry.Id = "eoh_town_" + NormalizeId(townName);
            entry.DisplayName = "Town: " + townName;
            entry.Type = "town";
            entry.OwnerGroupID = state.OwnerGroupID;
            entry.OwnerGroupName = state.OwnerGroupName;
            entry.TownName = townName;
            entry.Position = pos;
            results.Insert(entry);

            Print("[EoH_TerjeRespawn][REGISTRY] owned town respawn town=" + townName + " owner=" + state.OwnerGroupName + " pos=" + pos.ToString());
        }
    }

    static string NormalizeId(string raw)
    {
        string value = raw;
        value.Replace(" ", "_");
        value.ToLower();
        return value;
    }

    static void AddOwnedTerritoryRespawns(PlayerBase player, string playerGroupID, array<ref EoH_TerjeOwnedRespawnEntry> results)
    {
        if (!results || playerGroupID == "")
            return;

        array<Object> objects = new array<Object>();
        array<CargoBase> proxy = new array<CargoBase>();
        GetGame().GetObjectsAtPosition3D(player.GetPosition(), 30000, objects, proxy);

        foreach (Object obj : objects)
        {
            if (!obj || obj.GetType() != "TerritoryFlag")
                continue;

            string ownerGroupID = EoH_TerritoryOwnershipRegistry.GetOwnerForObject(obj);
            if (ownerGroupID == "" || ownerGroupID != playerGroupID)
                continue;

            EoH_TerjeOwnedRespawnEntry entry = new EoH_TerjeOwnedRespawnEntry();
            entry.Id = "eoh_territory_" + obj.GetID().ToString();
            entry.DisplayName = "Territory Flag";
            entry.Type = "territory";
            entry.OwnerGroupID = ownerGroupID;
            entry.OwnerGroupName = "Owned Territory";
            entry.Position = obj.GetPosition();
            results.Insert(entry);

            Print("[EoH_TerjeRespawn][REGISTRY] owned territory respawn pos=" + obj.GetPosition().ToString());
        }
    }

    static EoH_TerjeOwnedRespawnEntry FindById(PlayerBase player, string respawnId)
    {
        ref array<ref EoH_TerjeOwnedRespawnEntry> entries = BuildOwnedRespawns(player);
        foreach (EoH_TerjeOwnedRespawnEntry entry : entries)
        {
            if (entry && entry.Id == respawnId)
                return entry;
        }

        return null;
    }
}
