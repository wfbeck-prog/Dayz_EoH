class EoH_FieldReportMenu extends UIScriptedMenu
{
    protected TextWidget m_Title;
    protected TextWidget m_Subtitle;
    protected MultilineTextWidget m_Body;
    protected ButtonWidget m_CloseButton;
    protected ref EoH_FieldReportData m_Report;

    static void Open(EoH_FieldReportData report)
    {
        if (!GetGame() || !GetGame().GetUIManager() || !report)
            return;

        UIScriptedMenu current = GetGame().GetUIManager().GetMenu();
        if (current && current.IsInherited(EoH_FieldReportMenu))
            GetGame().GetUIManager().Back();

        EoH_FieldReportMenu menu = EoH_FieldReportMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(MENU_SCRIPTED, null));
        if (menu)
            menu.SetReport(report);
    }

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("EoH_Server/scripts/5_Mission/EoH_FieldReport/layouts/eoh_field_report.layout");

        m_Title = TextWidget.Cast(layoutRoot.FindAnyWidget("ReportTitle"));
        m_Subtitle = TextWidget.Cast(layoutRoot.FindAnyWidget("ReportSubtitle"));
        m_Body = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("ReportBody"));
        m_CloseButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CloseButton"));

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
            Close();
            return true;
        }

        return super.OnClick(w, x, y, button);
    }

    override void OnKeyPress(Widget w, int x, int y, int key)
    {
        super.OnKeyPress(w, x, y, key);

        if (key == KeyCode.KC_ESCAPE)
            Close();
    }

    void Close()
    {
        GetGame().GetUIManager().Back();
    }
};
