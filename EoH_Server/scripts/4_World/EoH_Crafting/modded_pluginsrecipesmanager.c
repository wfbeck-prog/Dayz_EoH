modded class PluginRecipesManagerBase
{
    override void RegisterRecipies()
    {
        super.RegisterRecipies();

        RegisterRecipe(new EoH_Craft_BBPBlueprint_FromPaper());

        Print("[EoH_Crafting] Registered recipe: Paper + Paper -> BBP_Blueprint");
    }
}
