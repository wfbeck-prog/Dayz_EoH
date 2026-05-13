class EoH_AtmosphereManager
{
    protected static ref EoH_AtmosphereManager s_Instance;
    protected ref EoH_AtmosphereConfig m_Config;
    protected int m_LastUpdate;

    static EoH_AtmosphereManager Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_AtmosphereManager();

        return s_Instance;
    }

    void EoH_AtmosphereManager()
    {
        m_Config = GetEoHAtmosphereConfig();
        m_LastUpdate = 0;
    }

    void Tick()
    {
        if (!GetGame().IsServer())
            return;

        if (!m_Config || m_Config.Enabled == 0)
            return;

        int now = GetGame().GetTime();
        if ((now - m_LastUpdate) < (m_Config.CheckIntervalSeconds * 1000.0))
            return;

        m_LastUpdate = now;
        ApplyAtmosphere();
    }

    void ApplyAtmosphere()
    {
        Weather weather = g_Game.GetWeather();
        if (!weather)
            return;

        float overcast = Math.RandomFloatInclusive(m_Config.OvercastMin, m_Config.OvercastMax);
        weather.GetOvercast().Set(overcast, m_Config.TransitionSeconds, 0);

        bool useMist = Math.RandomFloatInclusive(0.0, 1.0) <= m_Config.MistChance;

        float fogValue = 0.0;
        if (useMist)
            fogValue = Math.RandomFloatInclusive(m_Config.FogMin, m_Config.FogMax);

        weather.GetFog().Set(fogValue, m_Config.TransitionSeconds, 0);

        float rainValue = 0.0;
        if (Math.RandomFloatInclusive(0.0, 1.0) <= m_Config.RainChance)
            rainValue = Math.RandomFloatInclusive(m_Config.RainMin, m_Config.RainMax);

        weather.GetRain().Set(rainValue, m_Config.TransitionSeconds, 0);

        weather.SetWindMaximumSpeed(Math.RandomFloatInclusive(m_Config.WindMagnitudeMin, m_Config.WindMagnitudeMax));

        if (m_Config.DebugLogs == 1)
        {
            Print("[EoH_Atmosphere] Applied atmosphere overcast=" + overcast.ToString() + " fog=" + fogValue.ToString() + " rain=" + rainValue.ToString());
        }
    }
}
