class EoH_AtmosphereConfig
{
    int Enabled = 1;
    float MistChance = 0.70;
    float CheckIntervalSeconds = 900.0;
    float TransitionSeconds = 180.0;

    float OvercastMin = 0.70;
    float OvercastMax = 0.95;
    float FogMin = 0.35;
    float FogMax = 0.65;
    float RainChance = 0.15;
    float RainMin = 0.0;
    float RainMax = 0.25;
    float WindMagnitudeMin = 0.05;
    float WindMagnitudeMax = 0.35;

    int ForceWhiteMistOnly = 1;
    int LethalMist = 0;
    int DebugLogs = 1;

    void Normalize()
    {
        MistChance = Math.Clamp(MistChance, 0.0, 1.0);
        CheckIntervalSeconds = Math.Max(CheckIntervalSeconds, 60.0);
        TransitionSeconds = Math.Max(TransitionSeconds, 1.0);
        OvercastMin = Math.Clamp(OvercastMin, 0.0, 1.0);
        OvercastMax = Math.Clamp(OvercastMax, OvercastMin, 1.0);
        FogMin = Math.Clamp(FogMin, 0.0, 1.0);
        FogMax = Math.Clamp(FogMax, FogMin, 1.0);
        RainChance = Math.Clamp(RainChance, 0.0, 1.0);
        RainMin = Math.Clamp(RainMin, 0.0, 1.0);
        RainMax = Math.Clamp(RainMax, RainMin, 1.0);
        WindMagnitudeMin = Math.Clamp(WindMagnitudeMin, 0.0, 1.0);
        WindMagnitudeMax = Math.Clamp(WindMagnitudeMax, WindMagnitudeMin, 1.0);
    }
};

static const string EOH_ATMOSPHERE_DIR = "$profile:EoH_Server";
static const string EOH_ATMOSPHERE_FILE = "$profile:EoH_Server/EoH_AtmosphereConfig.json";

ref EoH_AtmosphereConfig g_EoH_AtmosphereConfig;

EoH_AtmosphereConfig GetEoHAtmosphereConfig()
{
    if (g_EoH_AtmosphereConfig)
        return g_EoH_AtmosphereConfig;

    g_EoH_AtmosphereConfig = new EoH_AtmosphereConfig();

    if (!FileExist(EOH_ATMOSPHERE_DIR))
        MakeDirectory(EOH_ATMOSPHERE_DIR);

    if (FileExist(EOH_ATMOSPHERE_FILE))
    {
        JsonFileLoader<EoH_AtmosphereConfig>.JsonLoadFile(EOH_ATMOSPHERE_FILE, g_EoH_AtmosphereConfig);
        g_EoH_AtmosphereConfig.Normalize();
        JsonFileLoader<EoH_AtmosphereConfig>.JsonSaveFile(EOH_ATMOSPHERE_FILE, g_EoH_AtmosphereConfig);
        return g_EoH_AtmosphereConfig;
    }

    g_EoH_AtmosphereConfig.Normalize();
    JsonFileLoader<EoH_AtmosphereConfig>.JsonSaveFile(EOH_ATMOSPHERE_FILE, g_EoH_AtmosphereConfig);
    return g_EoH_AtmosphereConfig;
}
