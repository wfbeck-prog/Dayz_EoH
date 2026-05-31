class EoH_AltarRepairProgressClient
{
    protected static ref EoH_AltarRepairProgressClient s_Instance;

    protected Widget m_Root;
    protected ProgressBarWidget m_ProgressBar;
    protected TextWidget m_TitleText;
    protected TextWidget m_StatusText;
    protected TextWidget m_ProgressText;
    protected TextWidget m_MembersText;
    protected TextWidget m_RadiusText;

    static EoH_AltarRepairProgressClient Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_AltarRepairProgressClient();

        return s_Instance;
    }

    void EoH_AltarRepairProgressClient()
    {
        GetRPCManager().AddRPC("EoH_AltarRepairProgress", "RPC_Show", this, SingleplayerExecutionType.Client);
        GetRPCManager().AddRPC("EoH_AltarRepairProgress", "RPC_Update", this, SingleplayerExecutionType.Client);
        GetRPCManager().AddRPC("EoH_AltarRepairProgress", "RPC_Hide", this, SingleplayerExecutionType.Client);
    }

    void RPC_Show(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Param1<EoH_AltarRepairProgressData> data;
        if (!ctx.Read(data))
            return;

        EnsureLayout();
        ApplyData(data.param1);
    }

    void RPC_Update(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Param1<EoH_AltarRepairProgressData> data;
        if (!ctx.Read(data))
            return;

        EnsureLayout();
        ApplyData(data.param1);
    }

    void RPC_Hide(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Hide();
    }

    protected void EnsureLayout()
    {
        if (m_Root)
            return;

        m_Root = GetGame().GetWorkspace().CreateWidgets("EoH_Server/scripts/5_Mission/EoH_WeeklyEvents/layouts/eoh_altar_repair_progress.layout");
        if (!m_Root)
            return;

        Class.CastTo(m_TitleText, m_Root.FindAnyWidget("TitleText"));
        Class.CastTo(m_StatusText, m_Root.FindAnyWidget("StatusText"));
        Class.CastTo(m_ProgressText, m_Root.FindAnyWidget("ProgressText"));
        Class.CastTo(m_MembersText, m_Root.FindAnyWidget("MembersText"));
        Class.CastTo(m_RadiusText, m_Root.FindAnyWidget("RadiusText"));
        Class.CastTo(m_ProgressBar, m_Root.FindAnyWidget("ProgressBar"));
    }

    protected void ApplyData(EoH_AltarRepairProgressData data)
    {
        if (!data)
            return;

        if (m_TitleText)
            m_TitleText.SetText(data.Title);

        if (m_StatusText)
            m_StatusText.SetText(data.Status);

        if (m_ProgressText)
            m_ProgressText.SetText(data.ProgressPercent.ToString() + "%");

        if (m_MembersText)
            m_MembersText.SetText("Repair team in zone: " + data.MembersInZone.ToString());

        if (m_RadiusText)
            m_RadiusText.SetText("Maintain radius: " + data.MaintainRadius.ToString() + "m");

        if (m_ProgressBar)
            m_ProgressBar.SetCurrent(data.ProgressPercent);

        if (data.Complete || data.Failed)
            GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(Hide, 4000, false);
    }

    void Hide()
    {
        if (!m_Root)
            return;

        m_Root.Unlink();
        m_Root = null;
        m_ProgressBar = null;
        m_TitleText = null;
        m_StatusText = null;
        m_ProgressText = null;
        m_MembersText = null;
        m_RadiusText = null;
    }
}

modded class MissionGameplay
{
    override void OnInit()
    {
        super.OnInit();
        EoH_AltarRepairProgressClient.Get();
    }
}
