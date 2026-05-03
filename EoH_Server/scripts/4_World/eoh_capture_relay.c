class EoH_CaptureRelay_Base extends Inventory_Base
{
    protected string m_EoH_TownName;

    void EoH_CaptureRelay_Base()
    {
        m_EoH_TownName = "";
    }

    void SetEoHTownName(string townName)
    {
        m_EoH_TownName = townName;
    }

    string GetEoHTownName()
    {
        return m_EoH_TownName;
    }

    override void SetActions()
    {
        super.SetActions();
        AddAction(EoH_ActionCaptureRelay);
    }
};
