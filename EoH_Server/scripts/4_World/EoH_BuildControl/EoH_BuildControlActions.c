modded class ActionDeployObject
{
    override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL)
    {
        if (!super.SetupAction(player, target, item, action_data, extra_data))
            return false;

        vector pos = GetPlacementPosition(player);

        if (!EoH_BuildControlManager.CanPlace(player, item.GetType(), pos))
            return false;

        return true;
    }

    protected vector GetPlacementPosition(PlayerBase player)
    {
        if (!player)
            return vector.Zero;

        EntityAI projection;

        if (GetGame().IsServer() && player.GetHologramServer())
            projection = player.GetHologramServer().GetProjectionEntity();
        else if (GetGame().IsClient() && player.GetHologramLocal())
            projection = player.GetHologramLocal().GetProjectionEntity();

        if (projection)
            return projection.GetPosition();

        return player.GetPosition();
    }
};

modded class ActionBuildPart
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!super.ActionCondition(player, target, item))
            return false;

        if (!target || !target.GetObject())
            return true;

        vector pos = target.GetObject().GetPosition();

        if (!EoH_BuildControlManager.CanPlace(player, item.GetType(), pos))
            return false;

        return true;
    }
};
