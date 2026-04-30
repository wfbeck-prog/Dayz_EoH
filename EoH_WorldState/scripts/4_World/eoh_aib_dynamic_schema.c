class EoH_AIBNPCProperties
{
    ref array<string> headgear;
    ref array<string> masks;
    ref array<string> vests;
    ref array<string> backpacks;
    ref array<string> bodywear;
    ref array<string> belts;
    ref array<string> pants;
    ref array<string> shoes;
    ref array<string> gloves;
    ref array<string> armband;
    ref array<string> loot;

    void EoH_AIBNPCProperties()
    {
        headgear = new array<string>();
        masks = new array<string>();
        vests = new array<string>();
        backpacks = new array<string>();
        bodywear = new array<string>();
        belts = new array<string>();
        pants = new array<string>();
        shoes = new array<string>();
        gloves = new array<string>();
        armband = new array<string>();
        loot = new array<string>();
    }
};

class EoH_AIBGroupLocation
{
    string name;
    string faction;
    ref array<string> waypoints;
    ref array<string> npcclasses;
    float accuracy;
    float grenadechance;
    int dog;
    ref array<string> weaponpool;
    ref EoH_AIBNPCProperties npcproperties;

    void EoH_AIBGroupLocation()
    {
        name = "";
        faction = "Bandits";
        waypoints = new array<string>();
        npcclasses = new array<string>();
        accuracy = 75.0;
        grenadechance = 0.0;
        dog = 0;
        weaponpool = new array<string>();
        npcproperties = new EoH_AIBNPCProperties();
    }
};

class EoH_AIBSniperLocation
{
    string name;
    ref array<string> positions;
    string npcclass;
    float accuracy;
    int fixedpos;
    string triggerpos;
    ref array<string> weaponpool;
    ref EoH_AIBNPCProperties npcproperties;

    void EoH_AIBSniperLocation()
    {
        name = "";
        positions = new array<string>();
        npcclass = "BanditAI_Keiko";
        accuracy = 75.0;
        fixedpos = 0;
        triggerpos = "0 0 0";
        weaponpool = new array<string>();
        npcproperties = new EoH_AIBNPCProperties();
    }
};

class EoH_AIBPredefinedWeapon
{
    string name;
    string weapon;
    ref array<string> attachments;

    void EoH_AIBPredefinedWeapon()
    {
        name = "";
        weapon = "";
        attachments = new array<string>();
    }
};

class EoH_AIBDynamicConfig
{
    int version;
    int showtriggers;
    int cleanzerovector;
    string crashsitegroup;
    int cardamage;
    ref array<ref EoH_AIBGroupLocation> GroupLocations;
    ref array<ref EoH_AIBSniperLocation> SniperLocations;
    ref array<ref EoH_AIBPredefinedWeapon> PredefinedWeapons;

    void EoH_AIBDynamicConfig()
    {
        version = 2;
        showtriggers = 0;
        cleanzerovector = 0;
        crashsitegroup = "Random";
        cardamage = 1;
        GroupLocations = new array<ref EoH_AIBGroupLocation>();
        SniperLocations = new array<ref EoH_AIBSniperLocation>();
        PredefinedWeapons = new array<ref EoH_AIBPredefinedWeapon>();
    }
};
