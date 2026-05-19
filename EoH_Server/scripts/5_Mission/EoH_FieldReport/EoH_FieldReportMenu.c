class EoH_FieldReportMenu extends UIScriptedMenu
{
    protected static ref EoH_FieldReportMenu s_ActiveReportMenu;

    protected TextWidget m_Title;
    protected TextWidget m_Subtitle;
    protected MultilineTextWidget m_Body;
    protected ButtonWidget m_CloseButton;
    protected ref EoH_FieldReportData m_Report;

    static void Open(EoH_FieldReportData report)
    {
        if (!GetGame() || !GetGame().GetWorkspace() || !report)
            return;

        if (s_ActiveReportMenu)
            s_ActiveReportMenu.EoH_DestroyReport();

        s_ActiveReportMenu = new EoH_FieldReportMenu();
        s_ActiveReportMenu.Init();
        s_ActiveReportMenu.SetReport(report);
    }

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("EoH_Server/scripts/5_Mission/EoH_FieldReport/layouts/eoh_field_report.layout");

        m_Title = TextWidget.Cast(layoutRoot.FindAnyWidget("ReportTitle"));
        m_Subtitle = TextWidget.Cast(layoutRoot.FindAnyWidget("ReportSubtitle"));
        m_Body = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("ReportBody"));
        m_CloseButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CloseButton"));

        if (layoutRoot)
            layoutRoot.Show(true);

        GetGame().GetUIManager().ShowUICursor(true);

        return layoutRoot;
    }

    void SetReport(EoH_FieldReportData report)
    {
        m_Report = report;

        if (!m_Report)
            return;

        if (m_Title)
            m_Title.SetText(m_Report.Title);

        if (m_Subtitle)
            m_Subtitle.SetText(m_Report.Subtitle);

        if (m_Body)
            m_Body.SetText(m_Report.Body);
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == m_CloseButton)
        {
            EoH_DestroyReport();
            return true;
        }

        return super.OnClick(w, x, y, button);
    }

    void EoH_DestroyReport()
    {
        if (layoutRoot)
        {
            layoutRoot.Unlink();
            layoutRoot = null;
        }

        GetGame().GetUIManager().ShowUICursor(false);

        if (s_ActiveReportMenu == this)
            s_ActiveReportMenu = null;
    }
};
