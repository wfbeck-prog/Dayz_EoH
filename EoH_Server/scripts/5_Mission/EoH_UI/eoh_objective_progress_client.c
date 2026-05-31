class EoH_ObjectiveProgressClient
{
    protected static ref EoH_ObjectiveProgressClient s_Instance;

    protected Widget m_Root;
    protected TextWidget m_TitleText;
    protected TextWidget m_StatusText;
    protected ProgressBarWidget m_ProgressBar;
    protected TextWidget m_ProgressText;
    protected TextWidget m_LeftText;
    protected TextWidget m_RightText;
    protected string m_ActiveChannel;

    static EoH_ObjectiveProgressClient Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_ObjectiveProgressClient();

        return s_Instance;
    }

    void EoH_ObjectiveProgressClient()
    {
        m_ActiveChannel = "";
        GetRPCManager().AddRPC("EoH_ObjectiveProgress", "RPC_Show", this, SingleplayerExecutionType.Client);
        GetRPCManager().AddRPC("EoH_ObjectiveProgress", "RPC_Update", this, SingleplayerExecutionType.Client);
        GetRPCManager().AddRPC("EoH_ObjectiveProgress", "RPC_Hide", this, SingleplayerExecutionType.Client);
    }

    void RPC_Show(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Param1<EoH_ObjectiveProgressData> dataParam;
        if (!ctx.Read(dataParam))
            return;

        Show(dataParam.param1);
    }

    void RPC_Update(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Param1<EoH_ObjectiveProgressData> dataParam;
        if (!ctx.Read(dataParam))
            return;

        Update(dataParam.param1);
    }

    void RPC_Hide(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Param1<string> channelParam;
        if (ctx.Read(channelParam))
            Hide(channelParam.param1);
        else
            Hide("");
    }

    void Show(EoH_ObjectiveProgressData data)
    {
        EnsureWidget();
        Update(data);

        if (m_Root)
            m_Root.Show(true);
    }

    void Update(EoH_ObjectiveProgressData data)
    {
        if (!data)
            return;

        data.Normalize();
        EnsureWidget();

        m_ActiveChannel = data.Channel;

        if (m_TitleText)
            m_TitleText.SetText(data.Title);
        if (m_StatusText)
            m_StatusText.SetText(data.Status);
        if (m_ProgressBar)
            m_ProgressBar.SetCurrent(data.ProgressPercent);
        if (m_ProgressText)
            m_ProgressText.SetText(data.ProgressPercent.ToString() + "%");
        if (m_LeftText)
            m_LeftText.SetText(data.LeftLabel);
        if (m_RightText)
            m_RightText.SetText(data.RightLabel);

        if (m_Root)
            m_Root.Show(true);
    }

    void Hide(string channel)
    {
        if (channel != "" && m_ActiveChannel != "" && channel != m_ActiveChannel)
            return;

        if (m_Root)
            m_Root.Show(false);

        m_ActiveChannel = "";
    }

    protected void EnsureWidget()
    {
        if (m_Root)
            return;

        m_Root = GetGame().GetWorkspace().CreateWidgets("EoH_Server/scripts/5_Mission/EoH_UI/layouts/eoh_objective_progress.layout");
        if (!m_Root)
        {
            Print("[EoH_ObjectiveProgressClient][WARN] Failed to create objective progress layout");
            return;
        }

        m_TitleText = TextWidget.Cast(m_Root.FindAnyWidget("TitleText"));
        m_StatusText = TextWidget.Cast(m_Root.FindAnyWidget("StatusText"));
        m_ProgressBar = ProgressBarWidget.Cast(m_Root.FindAnyWidget("ProgressBar"));
        m_ProgressText = TextWidget.Cast(m_Root.FindAnyWidget("ProgressText"));
        m_LeftText = TextWidget.Cast(m_Root.FindAnyWidget("LeftText"));
        m_RightText = TextWidget.Cast(m_Root.FindAnyWidget("RightText"));

        m_Root.Show(false);
    }
}
