class EoH_BookManager
{
    protected static ref EoH_BookManager s_Instance;

    protected ref EoH_BookConfig m_Config;
    protected ref map<string, ref array<string>> m_PlayerUnlockedEntries;

    static EoH_BookManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_BookManager();

        return s_Instance;
    }

    void EoH_BookManager()
    {
        m_PlayerUnlockedEntries = new map<string, ref array<string>>();
        Load();
    }

    void Load()
    {
        string path = "$profile:EoH_BookEntries.json";

        if (!FileExist(path))
        {
            CreateDefaultConfig(path);
        }

        m_Config = new EoH_BookConfig();
        JsonFileLoader<EoH_BookConfig>.JsonLoadFile(path, m_Config);

        Print("[EoH_Book] Loaded entries: " + m_Config.Entries.Count().ToString());
    }

    void CreateDefaultConfig(string path)
    {
        EoH_BookConfig cfg = new EoH_BookConfig();

        EoH_BookEntry intel = new EoH_BookEntry();
        intel.ID = "intel_blackmarket_001";
        intel.Title = "Black Market Rumors";
        intel.Category = "Intel";
        intel.Icon = "Book";
        intel.SortOrder = 1;
        intel.HiddenUntilDiscovered = true;
        intel.Body = "Survivors whisper of armed traders moving rare weapons through hidden routes at night.";
        cfg.Entries.Insert(intel);

        EoH_BookEntry relay = new EoH_BookEntry();
        relay.ID = "relay_capture_001";
        relay.Title = "Relay Network";
        relay.Category = "Territories";
        relay.Icon = "Radio";
        relay.SortOrder = 2;
        relay.HiddenUntilDiscovered = true;
        relay.Body = "Old military relays can still broadcast across Chernarus. Control the relay, control the information.";
        cfg.Entries.Insert(relay);

        EoH_BookEntry bunker = new EoH_BookEntry();
        bunker.ID = "bunker_lab_001";
        bunker.Title = "Underground Facility";
        bunker.Category = "Bunker";
        bunker.Icon = "Skull";
        bunker.SortOrder = 3;
        bunker.HiddenUntilDiscovered = true;
        bunker.Body = "Records indicate a sealed underground facility still exists somewhere in the north.";
        cfg.Entries.Insert(bunker);

        JsonFileLoader<EoH_BookConfig>.JsonSaveFile(path, cfg);

        Print("[EoH_Book] Created default book config");
    }

    bool UnlockEntry(PlayerBase player, string id)
    {
        if (!player || !player.GetIdentity())
            return false;

        string pid = player.GetIdentity().GetPlainId();

        if (!m_PlayerUnlockedEntries.Contains(pid))
            m_PlayerUnlockedEntries.Insert(pid, new array<string>());

        array<string> unlocked = m_PlayerUnlockedEntries.Get(pid);

        if (unlocked.Find(id) != -1)
            return false;

        unlocked.Insert(id);

        SendUnlockNotification(player, id);

        return true;
    }

    bool HasUnlocked(PlayerBase player, string id)
    {
        if (!player || !player.GetIdentity())
            return false;

        string pid = player.GetIdentity().GetPlainId();

        if (!m_PlayerUnlockedEntries.Contains(pid))
            return false;

        return m_PlayerUnlockedEntries.Get(pid).Find(id) != -1;
    }

    EoH_BookEntry GetEntry(string id)
    {
        foreach (EoH_BookEntry entry : m_Config.Entries)
        {
            if (entry.ID == id)
                return entry;
        }

        return null;
    }

    void SendUnlockNotification(PlayerBase player, string id)
    {
        EoH_BookEntry entry = GetEntry(id);
        if (!entry)
            return;

        NotificationSystem.SendNotificationToPlayerIdentityExtended(
            player.GetIdentity(),
            10,
            "Book Updated",
            entry.Title,
            "set:dayz_gui image:icon_info"
        );

        Print("[EoH_Book] Unlocked entry " + id + " for " + player.GetIdentity().GetName());
    }
}
