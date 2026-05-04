modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

        actions.Insert(ActionUseIntel);          // intel
        actions.Insert(EoH_ActionCaptureRelay);  // ✅ FIXED
    }
};
