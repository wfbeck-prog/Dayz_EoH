class EoH_FieldReportRPC
{
    // EOHR0. Personal read-only field report UI RPC.
    static const int OPEN_REPORT = 0x454F4852;
};

class EoH_FieldReportData
{
    string Title;
    string Subtitle;
    string Body;

    void EoH_FieldReportData(string title = "", string subtitle = "", string body = "")
    {
        Title = title;
        Subtitle = subtitle;
        Body = body;
    }
};
