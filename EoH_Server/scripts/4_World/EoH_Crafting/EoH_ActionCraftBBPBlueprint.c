class EoH_ActionCraftBBPBlueprintCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(2.0);
    }
};

class EoH_ActionCraftBBPBlueprint : ActionContinuousBase
{
    void EoH_ActionCraftBBPBlueprint()
    {
        m_CallbackClass = EoH_ActionCraftBBPBlueprintCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_FullBody = false;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
        m_Text = "Assemble BBP Blueprint";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTNone;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !item)
            return false;

        if (item.GetType() != "Paper")
            return false;

        int paperCount = 0;
        array<EntityAI> itemsArray = new array<EntityAI>();
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);

        foreach (EntityAI entity : itemsArray)
        {
            if (!entity)
                continue;

            if (entity.GetType() == "Paper")
                paperCount++;
        }

        return paperCount >= 2;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        if (!player)
            return;

        array<EntityAI> papers = new array<EntityAI>();
        array<EntityAI> itemsArray = new array<EntityAI>();
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);

        foreach (EntityAI entity : itemsArray)
        {
            if (!entity)
                continue;

            if (entity.GetType() == "Paper")
                papers.Insert(entity);
        }

        if (papers.Count() < 2)
            return;

        GetGame().ObjectDelete(papers[0]);
        GetGame().ObjectDelete(papers[1]);

        EntityAI blueprint = EntityAI.Cast(player.GetInventory().CreateInInventory("BBP_Blueprint"));

        if (!blueprint)
        {
            vector pos = player.GetPosition() + "0 0.2 0";
            blueprint = EntityAI.Cast(GetGame().CreateObject("BBP_Blueprint", pos));
        }

        Print("[EoH_Crafting] Crafted BBP_Blueprint from 2x Paper for player=" + player.GetIdentity().GetName());
    }
};

modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(EoH_ActionCraftBBPBlueprint);
    }
}
