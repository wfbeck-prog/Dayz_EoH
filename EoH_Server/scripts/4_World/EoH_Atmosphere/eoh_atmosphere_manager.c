class EoH_AtmosphereManager
{
    protected static ref EoH_AtmosphereManager s_Instance;
    protected ref EoH_AtmosphereConfig m_Config;
    protected int m_NextCycleTime;
    protected bool m_Initialized;
    protected bool m_CurrentMistCycle;

    static EoH_AtmosphereManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_AtmosphereManager();

        return s_Instance;
    }

    void EoH_AtmosphereManager()
    {
        m_Config = GetEoHAtmosphereConfig();
        m_NextCycleTime = 0;
        m_Initialized = false;
        m_CurrentMistCycle = false;

        if (m_Config && m_Config.DebugLogs == 1)
            Print("[EoH_Atmosphere] Manager created. Enabled=" + m_Config.Enabled.ToString() + " MistChance=" + m_Config.MistChance.ToString());
    }

    void Tick()
    {
        if (!GetGame().IsServer())
            return;

        if (!m_Config)
            m_Config = GetEoHAtmosphereConfig();

        if (!m_Config || m_Config.Enabled == 0)
            return;

        int now = GetGame().GetTime();

        if (!m_Initialized)
        {
            m_Initialized = true;
            StartNewCycle(now, true);
            return;
        }

        if (now < m_NextCycleTime)
            return;

        StartNewCycle(now, false);
    }

    void StartNewCycle(int now, bool startup)
    {
        bool useMist = Math.RandomFloatInclusive(0.0, 1.0) <= m_Config.MistChance;
        m_CurrentMistCycle = useMist;

        float durationMinutes;
        if (useMist)
            durationMinutes = Math.RandomFloatInclusive(m_Config.MistDurationMinutesMin, m_Config.MistDurationMinutesMax);
        else
            durationMinutes = Math.RandomFloatInclusive(m_Config.ClearDurationMinutesMin, m_Config.ClearDurationMinutesMax);

        m_NextCycleTime = now + Math.Round(durationMinutes * 60.0 * 1000.0);

        if (useMist)
            ApplyMistCycle(startup);
        else
            ApplyClearCycle(startup);

        if (m_Config.DebugLogs == 1)
        {
            string cycleName = "CLEAR";
            if (useMist)
                cycleName = "MIST";

            Print("[EoH_Atmosphere] Started " + cycleName + " cycle durationMinutes=" + durationMinutes.ToString() + " startup=" + startup.ToString());
        }
    }

    void ApplyMistCycle(bool startup)
    {
        Weather weather = g_Game.GetWeather();
        if (!weather)
        {
            Print("[EoH_Atmosphere][WARN] Weather object unavailable during mist cycle.");
            return;
        }

        float overcast = Math.RandomFloatInclusive(m_Config.MistOvercastMin, m_Config.MistOvercastMax);
        float fogValue = Math.RandomFloatInclusive(m_Config.MistFogMin, m_Config.MistFogMax);
        float rainValue = 0.0;
        if (Math.RandomFloatInclusive(0.0, 1.0) <= m_Config.RainChance)
            rainValue = Math.RandomFloatInclusive(m_Config.RainMin, m_Config.RainMax);

        ApplyWeatherValues(weather, overcast, fogValue, rainValue, startup);
    }

    void ApplyClearCycle(bool startup)
    {
        Weather weather = g_Game.GetWeather();
        if (!weather)
        {
            Print("[EoH_Atmosphere][WARN] Weather object unavailable during clear cycle.");
            return;
        }

        float overcast = Math.RandomFloatInclusive(m_Config.ClearOvercastMin, m_Config.ClearOvercastMax);
        float fogValue = Math.RandomFloatInclusive(m_Config.ClearFogMin, m_Config.ClearFogMax);
        float rainValue = 0.0;

        ApplyWeatherValues(weather, overcast, fogValue, rainValue, startup);
    }

    void ApplyWeatherValues(Weather weather, float overcast, float fogValue, float rainValue, bool startup)
    {
        float transition = m_Config.TransitionSeconds;
        if (startup)
            transition = 30.0;

        weather.GetOvercast().Set(overcast, transition, 0);
        weather.GetFog().Set(fogValue, transition, 0);
        weather.GetRain().Set(rainValue, transition, 0);
        weather.SetWindMaximumSpeed(Math.RandomFloatInclusive(m_Config.WindMagnitudeMin, m_Config.WindMagnitudeMax));

        if (m_Config.DebugLogs == 1)
            Print("[EoH_Atmosphere] Applied weather overcast=" + overcast.ToString() + " fog=" + fogValue.ToString() + " rain=" + rainValue.ToString() + " transition=" + transition.ToString());
    }
}
