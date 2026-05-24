class EoH_DNACrateRelocatorRuntime
{
    string ClassName;
    int NextMoveTime;

    void EoH_DNACrateRelocatorRuntime(string className)
    {
        ClassName = className;
        NextMoveTime = 0;
    }
}

class EoH_DNACrateRelocator
{
    protected static ref EoH_DNACrateRelocator s_Instance;

    protected ref map<string, ref EoH_DNACrateRelocatorRuntime> m_Runtime;
    protected int m_LastTick;

    static const int TICK_MS = 60000;
    static const int MIN_MOVE_MS = 1800000;
    static const int MAX_MOVE_MS = 3600000;
    static const float SCAN_RADIUS = 30000.0;

    static EoH_DNACrateRelocator Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_DNACrateRelocator();

        return s_Instance;
    }

    void EoH_DNACrateRelocator()
    {
        m_Runtime = new map<string, ref EoH_DNACrateRelocatorRuntime>();
        RegisterCrate("DNA_Crate_Yellow");
        RegisterCrate("DNA_Crate_Green");
        RegisterCrate("DNA_Crate_Blue");
        RegisterCrate("DNA_Crate_Red");
        m_LastTick = 0;
        Print("[EoH_DNA] DNA crate relocator initialized. Purple crates are excluded.");
    }

    void RegisterCrate(string className)
    {
        if (className == "")
            return;

        if (!m_Runtime.Contains(className))
            m_Runtime.Set(className, new EoH_DNACrateRelocatorRuntime(className));
    }

    void Tick()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        int now = GetGame().GetTime();
        if (m_LastTick > 0 && now - m_LastTick < TICK_MS)
            return;

        m_LastTick = now;

        foreach (string className, EoH_DNACrateRelocatorRuntime runtime : m_Runtime)
        {
            if (!runtime)
                continue;

            if (runtime.NextMoveTime <= 0)
            {
                runtime.NextMoveTime = now + Math.RandomIntInclusive(MIN_MOVE_MS, MAX_MOVE_MS);
                Print("[EoH_DNA] Scheduled first relocation class=" + className + " nextMs=" + runtime.NextMoveTime.ToString());
                continue;
            }

            if (now < runtime.NextMoveTime)
                continue;

            RelocateCratesOfType(className);
            runtime.NextMoveTime = now + Math.RandomIntInclusive(MIN_MOVE_MS, MAX_MOVE_MS);
            Print("[EoH_DNA] Scheduled next relocation class=" + className + " nextMs=" + runtime.NextMoveTime.ToString());
        }
    }

    void RelocateCratesOfType(string className)
    {
        array<Object> objects = new array<Object>();
        array<CargoBase> cargos = new array<CargoBase>();
        GetGame().GetObjectsAtPosition3D("7500 0 7500", SCAN_RADIUS, objects, cargos);

        int moved = 0;
        foreach (Object obj : objects)
        {
            if (!obj || obj.GetType() != className)
                continue;

            vector newPos = GetRandomConfiguredPositionForClass(className);
            if (newPos == "0 0 0".ToVector())
                continue;

            newPos[1] = GetGame().SurfaceY(newPos[0], newPos[2]) + 0.05;
            obj.SetPosition(newPos);
            obj.PlaceOnSurface();
            moved++;
        }

        Print("[EoH_DNA] Relocated DNA crates class=" + className + " moved=" + moved.ToString());
    }

    vector GetRandomConfiguredPositionForClass(string className)
    {
        // Phase 1 scaffold: use broad EoH-safe fallback pools until we parse/import the DNA config arrays directly.
        // Purple is intentionally never registered and never moved.
        array<vector> pool = new array<vector>();

        if (className == "DNA_Crate_Yellow")
        {
            pool.Insert("13472.18 6.2 6281.44".ToVector());
            pool.Insert("7544.91 6.4 5150.88".ToVector());
            pool.Insert("6412.11 7.3 2661.17".ToVector());
            pool.Insert("2855.66 7.0 5281.41".ToVector());
            pool.Insert("8122.55 6.3 10484.74".ToVector());
        }
        else if (className == "DNA_Crate_Green")
        {
            pool.Insert("11352.44 225.1 6604.71".ToVector());
            pool.Insert("7612.91 155.2 11951.11".ToVector());
            pool.Insert("5221.91 144.7 10782.11".ToVector());
            pool.Insert("3241.77 148.1 10941.88".ToVector());
            pool.Insert("9732.88 188.4 12482.77".ToVector());
        }
        else if (className == "DNA_Crate_Blue")
        {
            pool.Insert("6684.261719 377.631348 14494.492188".ToVector());
            pool.Insert("5841.708984 230.093140 13541.537109".ToVector());
            pool.Insert("3869.527100 188.614426 13015.187500".ToVector());
            pool.Insert("6946.701660 373.448792 11694.593750".ToVector());
            pool.Insert("5302.299316 336.935425 9789.220703".ToVector());
        }
        else if (className == "DNA_Crate_Red")
        {
            pool.Insert("1665.077637 452.178131 14395.183594".ToVector());
            pool.Insert("1680.110229 448.389740 14211.728516".ToVector());
            pool.Insert("1535.075317 452.562866 14172.142578".ToVector());
            pool.Insert("1891.144043 448.797119 13704.332031".ToVector());
            pool.Insert("3697.671631 364.566498 14825.053711".ToVector());
        }

        if (pool.Count() == 0)
            return "0 0 0".ToVector();

        return pool.Get(Math.RandomInt(0, pool.Count()));
    }
}
