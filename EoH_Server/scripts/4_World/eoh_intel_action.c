class ActionUseIntel: ActionSingleUseBase
{
    void ActionUseIntel()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
    }

    override string GetText()
    {
        return "Use Intel";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !item)
            return false;

        string type = item.GetType();
        return type == "EoH_Intel_Document" || type == "EoH_TownIntel" || type == "EoH_TraderIntel";
    }

    override void OnExecuteServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        ItemBase intel = action_data.m_MainItem;

        if (!player || !intel)
            return;

        string type = intel.GetType();

        if (type == "EoH_TraderIntel")
        {
            EoH_IntelManager.Get().RevealTraderIntel(player);
        }
        else
        {
            EoH_IntelManager.Get().RevealTownIntel(player);
        }

        intel.Delete();
    }
};

modded class EoH_Intel_Document
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionUseIntel);
    }
};

modded class EoH_TownIntel
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionUseIntel);
    }
};

modded class EoH_TraderIntel
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionUseIntel);
    }
};
