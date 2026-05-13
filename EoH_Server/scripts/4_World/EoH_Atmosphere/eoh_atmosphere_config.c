class EoH_AtmosphereConfig
{
    int Enabled = 1;

    // Percent chance that the next atmosphere cycle will be a mist cycle.
    float MistChance = 0.70;

    // Legacy/simple interval fallback. The manager now uses mist/clear cycle durations.
    float CheckIntervalSeconds = 900.0;
    float TransitionSeconds = 180.0;

    // Mist cycle duration range.
    float MistDurationMinutesMin = 20.0;
    float MistDurationMinutesMax = 60.0;

    // Clear/relief cycle duration range.
    float ClearDurationMinutesMin = 15.0;
    float ClearDurationMinutesMax = 45.0;

    // Gloomy mist weather.
    float MistOvercastMin = 0.70;
    float MistOvercastMax = 0.95;
    float MistFogMin = 0.35;
    float MistFogMax = 0.65;

    // Clearer relief weather.
    float ClearOvercastMin = 0.35;
    float ClearOvercastMax = 0.65;
    float ClearFogMin = 0.02;
    float ClearFogMax = 0.18;

    // Backward-compatible values used as default mist values.
    float OvercastMin = 0.70;
    float OvercastMax = 0.95;
    float FogMin = 0.35;
    float FogMax = 0.65;

    float RainChance = 0.15;
    float RainMin = 0.0;
    float RainMax = 0.25;
    float WindMagnitudeMin = 0.05;
    float WindMagnitudeMax = 0.35;

    // Particle mist is config-supported but not force-enabled by default.
    // Use weather fog as the safe baseline. Particle spawning should only be enabled after runtime validation.
    int UseParticleMist = 0;
    string ParticleName = "graphics/particles/spooky_mist";
    ref array<float> ParticleCenter = {7200.0, 0.0, 8400.0};
    float ParticleRadius = 8500.0;
    float ParticlePosHeight = 22.0;
    float ParticleNegHeight = 10.0;

    int ForceWhiteMistOnly = 1;
    int LethalMist = 0;
    int DebugLogs = 1;

    void Normalize()
    {
        MistChance = Math.Clamp(MistChance, 0.0, 1.0);
        CheckIntervalSeconds = Math.Max(CheckIntervalSeconds, 60.0);
        TransitionSeconds = Math.Max(TransitionSeconds, 1.0);

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
