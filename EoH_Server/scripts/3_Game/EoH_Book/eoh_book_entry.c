class EoH_BookEntry
{
    string ID;
    string Title;
    string Category;
    string Icon;
    string Body;
    int SortOrder;
    bool HiddenUntilDiscovered;

    void EoH_BookEntry()
    {
        ID = "";
        Title = "";
        Category = "Intel";
        Icon = "Info";
        Body = "";
        SortOrder = 0;
        HiddenUntilDiscovered = false;
    }
}

class EoH_BookConfig
{
    int Version;
    ref array<ref EoH_BookEntry> Entries;

    void EoH_BookConfig()
    {
        Version = 1;
        Entries = new array<ref EoH_BookEntry>();
    }
}
