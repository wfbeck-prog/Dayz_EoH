modded class TerjeSpawnSelector
{
    override void BuildSpawnList(PlayerBase player, ref array<ref TerjeSpawnOption> options)
    {
        super.BuildSpawnList(player, options);

        vector flagPos;

        if (EoH_TerritorySpawn.GetGroupFlagPosition(player, flagPos))
        {
            string reason;

            if (!EoH_SpawnProtection.CanSpawn(player, flagPos, reason))
            {
                TerjeSpawnOption blocked = new TerjeSpawnOption();
                blocked.Name = "Group Territory (LOCKED)";
                blocked.Description = reason;
                blocked.Position = "0 0 0";
                options.Insert(blocked);
                return;
            }

            TerjeSpawnOption opt = new TerjeSpawnOption();

            opt.Name = "Group Territory";
            opt.Description = "Spawn near your group's territory.";
            opt.Position = EoH_TerritorySpawn.GetSafeSpawn(flagPos);

            options.Insert(opt);
        }
    }

    override void OnSpawnSelected(PlayerBase player, TerjeSpawnOption option)
    {
        super.OnSpawnSelected(player, option);

        if (option && option.Name == "Group Territory")
        {
            EoH_SpawnProtection.RegisterSpawn(player);
        }
    }
};
