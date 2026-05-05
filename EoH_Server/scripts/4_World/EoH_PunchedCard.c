modded class PunchedCard : Paper
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionUseUndergroundPanel);
    }
}
