modded class TerjeSpawnSelector
{
    override void BuildSpawnList(PlayerBase player, ref array<ref TerjeSpawnOption> options)
    {
        super.BuildSpawnList(player, options);

        vector flagPos;

        if (EoH_TerritorySpawn.GetGroupFlagPosition(player, flagPos))
        {
            TerjeSpawnOption opt = new TerjeSpawnOption();

            opt.Name = "Group Territory";
            opt.Description = "Spawn near your group's territory.";
            opt.Position = EoH_TerritorySpawn.GetSafeSpawn(flagPos);

            options.Insert(opt);
        }
    }
};
