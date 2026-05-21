modded class PluginTerjeStartScreen
{
    override void BuildRespawnsForPlayer(PlayerBase player, out TerjeXmlObject result)
    {
        super.BuildRespawnsForPlayer(player, result);

        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!player || !player.GetIdentity() || !result)
            return;

        ref array<ref EoH_TerjeOwnedRespawnEntry> entries = EoH_TerjeOwnedRespawnRegistry.BuildOwnedRespawns(player);
        foreach (EoH_TerjeOwnedRespawnEntry entry : entries)
        {
            if (!entry || entry.Id == "" || entry.Position == "0 0 0".ToVector())
                continue;

            EoH_AddOwnedRespawnXml(result, entry);
        }

        Print("[EoH_TerjeRespawn][STARTSCREEN] Injected owned respawns player=" + player.GetIdentity().GetName() + " count=" + entries.Count().ToString());
    }

    void EoH_AddOwnedRespawnXml(TerjeXmlObject root, EoH_TerjeOwnedRespawnEntry entry)
    {
        if (!root || !entry)
            return;

        TerjeXmlObject respawn = root.CreateChild("Respawn");
        if (!respawn)
            return;

        respawn.SetAttribute("id", entry.Id);
        respawn.SetAttribute("displayName", entry.DisplayName);

        TerjeXmlObject options = respawn.CreateChild("Options");
        if (options)
        {
            TerjeXmlObject mapRender = options.CreateChild("MapRender");
            if (mapRender)
            {
                mapRender.SetAttribute("pos", EoH_VectorToTerjePos(entry.Position));
                mapRender.SetAttribute("zoom", "0.25");
                mapRender.SetAttribute("showMarker", "always");
                mapRender.SetAttribute("showPoints", "always");
                mapRender.SetAttribute("allowInteraction", "1");
            }

            TerjeXmlObject safe = options.CreateChild("SafeRadius");
            if (safe)
            {
                safe.SetAttribute("zombie", "15");
                safe.SetAttribute("animal", "0");
                safe.SetAttribute("player", "100");
                safe.SetAttribute("other", "0");
            }
        }

        TerjeXmlObject points = respawn.CreateChild("Points");
        if (points)
        {
            TerjeXmlObject point = points.CreateChild("Point");
            if (point)
                point.SetAttribute("pos", EoH_VectorToTerjePos(entry.Position));
        }

        TerjeXmlObject conditions = respawn.CreateChild("Conditions");
        if (conditions)
        {
            TerjeXmlObject timeout = conditions.CreateChild("Timeout");
            if (timeout)
            {
                timeout.SetAttribute("id", entry.Id);
                timeout.SetAttribute("minutes", "10");
            }
        }

        Print("[EoH_TerjeRespawn][STARTSCREEN] Added respawn id=" + entry.Id + " name=" + entry.DisplayName + " type=" + entry.Type + " pos=" + entry.Position.ToString());
    }

    string EoH_VectorToTerjePos(vector pos)
    {
        return pos[0].ToString() + ", " + pos[2].ToString();
    }
}
