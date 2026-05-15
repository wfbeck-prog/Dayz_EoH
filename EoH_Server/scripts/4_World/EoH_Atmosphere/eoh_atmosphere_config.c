class EoH_AtmosphereConfig
{
    int Enabled = 1;
    int ForceWeatherControl = 1;
    int DebugForceMist = 1;
    float ForceReapplySeconds = 120.0;

    // Percent chance that the next atmosphere cycle will be a mist cycle.
    float MistChance = 0.70;

    // Legacy/simple interval fallback. The manager now uses mist/clear cycle durations.
    float CheckIntervalSeconds = 900.0;
    float TransitionSeconds = 180.0;

    // Fast startup/test transition. Used when DebugForceMist is enabled or on first apply.
    float StartupTransitionSeconds = 10.0;

    // Mist cycle duration range.
    float MistDurationMinutesMin = 20.0;
    float MistDurationMinutesMax = 60.0;

    // Clear/relief cycle duration range.
    float ClearDurationMinutesMin = 15.0;
    float ClearDurationMinutesMax = 45.0;

    // Gloomy mist weather.
    float MistOvercastMin = 0.90;
    float MistOvercastMax = 1.0;
    float MistFogMin = 0.75;
    float MistFogMax = 0.95;

    // Hard diagnostic mist values. Leave DebugForceMist=1 until weather control is visually confirmed.
    float DebugMistOvercast = 1.0;
    float DebugMistFog = 0.95;

    // Clearer relief weather.
    float ClearOvercastMin = 0.45;
    float ClearOvercastMax = 0.70;
    float ClearFogMin = 0.05;
    float ClearFogMax = 0.20;

    // Backward-compatible values used as default mist values.
    float OvercastMin = 0.90;
    float OvercastMax = 1.0;
    float FogMin = 0.75;
    float FogMax = 0.95;

    float RainChance = 0.0;
    float RainMin = 0.0;
    float RainMax = 0.0;
    float WindMagnitudeMin = 0.05;
    float WindMagnitudeMax = 0.25;

    // Optional white particle mist overlay. This is non-lethal and separate from contaminated area damage/PPE.
    // Enable this when weather fog is not visually dense enough.
    int UseParticleMist = 1;
    string ParticleName = "graphics/particles/spooky_mist";
    ref array<float> ParticleCenter = {7200.0, 0.0, 8400.0};
    float ParticleRadius = 8500.0;
    float ParticlePosHeight = 22.0;
    float ParticleNegHeight = 10.0;
    int ParticleGridStep = 900;
    int ParticleMaxEmitters = 80;

    int ForceWhiteMistOnly = 1;
    int LethalMist = 0;
    int DebugLogs = 1;

    void Normalize()
    {
        MistChance = Math.Clamp(MistChance, 0.0, 1.0);
        CheckIntervalSeconds = Math.Max(CheckIntervalSeconds, 60.0);
        TransitionSeconds = Math.Max(TransitionSeconds, 1.0);
        StartupTransitionSeconds = Math.Max(StartupTransitionSeconds, 1.0);
        ForceReapplySeconds = Math.Max(ForceReapplySeconds, 30.0);

        MistDurationMinutesMin = Math.Max(MistDurationMinutesMin, 1.0);
        MistDurationMinutesMax = Math.Max(MistDurationMinutesMax, MistDurationMinutesMin);
        ClearDurationMinutesMin = Math.Max(ClearDurationMinutesMin, 1.0);
        ClearDurationMinutesMax = Math.Max(ClearDurationMinutesMax, ClearDurationMinutesMin);

        OvercastMin = Math.Clamp(OvercastMin, 0.0, 1.0);
        OvercastMax = Math.Clamp(OvercastMax, OvercastMin, 1.0);
        FogMin = Math.Clamp(FogMin, 0.0, 1.0);
        FogMax = Math.Clamp(FogMax, FogMin, 1.0);

        MistOvercastMin = Math.Clamp(MistOvercastMin, 0.0, 1.0);
        MistOvercastMax = Math.Clamp(MistOvercastMax, MistOvercastMin, 1.0);
        MistFogMin = Math.Clamp(MistFogMin, 0.0, 1.0);
        MistFogMax = Math.Clamp(MistFogMax, MistFogMin, 1.0);
        DebugMistOvercast = Math.Clamp(DebugMistOvercast, 0.0, 1.0);
        DebugMistFog = Math.Clamp(DebugMistFog, 0.0, 1.0);

        ClearOvercastMin = Math.Clamp(ClearOvercastMin, 0.0, 1.0);
        ClearOvercastMax = Math.Clamp(ClearOvercastMax, ClearOvercastMin, 1.0);
        ClearFogMin = Math.Clamp(ClearFogMin, 0.0, 1.0);
        ClearFogMax = Math.Clamp(ClearFogMax, ClearFogMin, 1.0);

        RainChance = Math.Clamp(RainChance, 0.0, 1.0);
        RainMin = Math.Clamp(RainMin, 0.0, 1.0);
        RainMax = Math.Clamp(RainMax, RainMin, 1.0);
        WindMagnitudeMin = Math.Clamp(WindMagnitudeMin, 0.0, 1.0);
        WindMagnitudeMax = Math.Clamp(WindMagnitudeMax, WindMagnitudeMin, 1.0);

        ParticleRadius = Math.Max(ParticleRadius, 100.0);
        ParticlePosHeight = Math.Max(ParticlePosHeight, 1.0);
        ParticleNegHeight = Math.Max(ParticleNegHeight, 1.0);
        ParticleGridStep = Math.Max(ParticleGridStep, 250);
        ParticleMaxEmitters = Math.Clamp(ParticleMaxEmitters, 1, 250);

        if (!ParticleCenter || ParticleCenter.Count() != 3)
        {
            ParticleCenter = new array<float>();
            ParticleCenter.Insert(7200.0);
            ParticleCenter.Insert(0.0);
            ParticleCenter.Insert(8400.0);
        }
    }

    vector GetParticleCenterVector()
    {
        if (!ParticleCenter || ParticleCenter.Count() != 3)
            return "7200 0 8400".ToVector();

        return Vector(ParticleCenter.Get(0), ParticleCenter.Get(1), ParticleCenter.Get(2));
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
