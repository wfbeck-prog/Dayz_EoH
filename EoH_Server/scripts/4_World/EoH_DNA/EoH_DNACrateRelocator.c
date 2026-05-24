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

    static const bool ENABLED = true;
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
        Print("[EoH_DNA] DNA crate relocator initialized. Yellow/Green/Blue/Red rotate. Purple crates are excluded.");
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
        if (!ENABLED)
            return;

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

            vector oldPos = obj.GetPosition();
            vector newPos = GetRandomKnownIntelSafePositionForClass(className, oldPos);
            if (newPos == "0 0 0".ToVector())
                continue;

            newPos[1] = GetGame().SurfaceY(newPos[0], newPos[2]) + 0.05;
            obj.SetPosition(newPos);
            obj.PlaceOnSurface();
            moved++;

            Print("[EoH_DNA] Relocated crate class=" + className + " old=" + oldPos.ToString() + " new=" + newPos.ToString() + " intelSafe=true");
        }

        Print("[EoH_DNA] Relocated DNA crates class=" + className + " moved=" + moved.ToString());
    }

    vector GetRandomKnownIntelSafePositionForClass(string className, vector oldPos)
    {
        array<vector> pool = new array<vector>();
        FillKnownIntelSafePool(className, pool);

        if (pool.Count() == 0)
            return "0 0 0".ToVector();

        if (pool.Count() == 1)
            return pool.Get(0);

        vector selected = pool.Get(Math.RandomInt(0, pool.Count()));
        int guard = 0;
        while (vector.Distance(selected, oldPos) < 2.0 && guard < 10)
        {
            selected = pool.Get(Math.RandomInt(0, pool.Count()));
            guard++;
        }

        return selected;
    }

    void FillKnownIntelSafePool(string className, array<vector> pool)
    {
        if (!pool)
            return;

        if (className == "DNA_Crate_Yellow")
        {
            pool.Insert("13472.18 6.2 6281.44".ToVector());
            pool.Insert("13171.45 6.1 7074.91".ToVector());
            pool.Insert("12057.82 6.5 9084.55".ToVector());
            pool.Insert("10488.64 6.3 9733.17".ToVector());
            pool.Insert("10021.47 7.0 11634.11".ToVector());
            pool.Insert("8705.22 8.0 13312.42".ToVector());
            pool.Insert("8202.11 6.7 12485.55".ToVector());
            pool.Insert("7544.91 6.4 5150.88".ToVector());
            pool.Insert("6890.12 6.8 2483.91".ToVector());
            pool.Insert("6592.74 7.1 3578.44".ToVector());
            pool.Insert("6412.11 7.3 2661.17".ToVector());
            pool.Insert("6124.82 7.4 7772.19".ToVector());
            pool.Insert("5930.74 6.8 10223.18".ToVector());
            pool.Insert("5314.55 7.0 8612.27".ToVector());
            pool.Insert("4722.15 7.2 6781.90".ToVector());
            pool.Insert("4322.91 7.5 8272.44".ToVector());
            pool.Insert("3781.88 6.6 5984.11".ToVector());
            pool.Insert("3520.44 6.7 4874.55".ToVector());
            pool.Insert("3312.78 7.1 3931.55".ToVector());
            pool.Insert("2855.66 7.0 5281.41".ToVector());
            pool.Insert("2451.88 6.8 6322.91".ToVector());
            pool.Insert("2240.17 6.9 7512.62".ToVector());
            pool.Insert("1912.15 7.3 9110.82".ToVector());
            pool.Insert("1688.91 7.4 10284.17".ToVector());
            pool.Insert("1433.51 7.6 11772.88".ToVector());
            pool.Insert("1011.55 7.1 12682.47".ToVector());
            pool.Insert("4120.82 6.9 11344.17".ToVector());
            pool.Insert("5220.61 6.5 11918.73".ToVector());
            pool.Insert("7331.11 6.8 11284.28".ToVector());
            pool.Insert("8122.55 6.3 10484.74".ToVector());
            pool.Insert("9455.11 6.4 8820.44".ToVector());
            pool.Insert("10451.11 6.7 7355.88".ToVector());
            pool.Insert("11224.91 6.5 6551.77".ToVector());
            pool.Insert("12155.61 6.4 5442.22".ToVector());
            pool.Insert("12712.44 6.3 4781.11".ToVector());
            pool.Insert("13351.22 6.2 4142.66".ToVector());
        }
        else if (className == "DNA_Crate_Green")
        {
            pool.Insert("11352.44 225.1 6604.71".ToVector());
            pool.Insert("10471.55 195.4 8033.15".ToVector());
            pool.Insert("9861.44 182.3 8832.88".ToVector());
            pool.Insert("9142.11 167.7 10125.47".ToVector());
            pool.Insert("8441.17 160.5 11282.91".ToVector());
            pool.Insert("7612.91 155.2 11951.11".ToVector());
            pool.Insert("6744.82 150.1 12211.91".ToVector());
            pool.Insert("5933.22 148.4 11552.61".ToVector());
            pool.Insert("5221.91 144.7 10782.11".ToVector());
            pool.Insert("4410.55 138.6 9921.44".ToVector());
            pool.Insert("3742.17 132.5 9182.88".ToVector());
            pool.Insert("3112.91 128.1 8512.11".ToVector());
            pool.Insert("2521.44 125.7 7724.55".ToVector());
            pool.Insert("1981.11 120.4 6911.88".ToVector());
            pool.Insert("1452.61 117.7 6024.11".ToVector());
            pool.Insert("3241.77 148.1 10941.88".ToVector());
            pool.Insert("4321.15 153.5 11812.77".ToVector());
            pool.Insert("5311.44 161.2 12584.17".ToVector());
            pool.Insert("6422.55 168.4 13211.41".ToVector());
            pool.Insert("7521.77 174.2 13782.11".ToVector());
            pool.Insert("8611.41 181.7 13255.55".ToVector());
            pool.Insert("9732.88 188.4 12482.77".ToVector());
            pool.Insert("10822.41 201.2 11612.15".ToVector());
            pool.Insert("11741.55 214.1 10284.91".ToVector());
        }
        else if (className == "DNA_Crate_Blue")
        {
            pool.Insert("6684.26171875 377.63134765625 14494.4921875".ToVector());
            pool.Insert("5841.708984375 230.093139648438 13541.537109375".ToVector());
            pool.Insert("7473.900390625 214.951629638672 13534.20703125".ToVector());
            pool.Insert("3869.52709960938 188.61442565918 13015.1875".ToVector());
            pool.Insert("3018.50903320312 211.657821655273 12540.15625".ToVector());
            pool.Insert("1419.88671875 284.16552734375 11994.841796875".ToVector());
            pool.Insert("7734.7041015625 126.264305114746 12638.8505859375".ToVector());
            pool.Insert("9607.2880859375 83.4399490356445 13701.396484375".ToVector());
            pool.Insert("4042.35815429688 372.708618164062 11712.103515625".ToVector());
            pool.Insert("2220.57275390625 266.157470703125 11069.279296875".ToVector());
            pool.Insert("6946.70166015625 373.448791503906 11694.59375".ToVector());
            pool.Insert("486.411468505859 421.826538085938 11071.248046875".ToVector());
            pool.Insert("2723.33520507812 274 9963.9873046875".ToVector());
            pool.Insert("1576.28930664062 302.384613037109 8972.732421875".ToVector());
            pool.Insert("4557.0859375 338.978485107422 10742.880859375".ToVector());
            pool.Insert("4130.24072265625 339.400360107422 10431.693359375".ToVector());
            pool.Insert("4519.34228515625 339.1533203125 9646.5859375".ToVector());
            pool.Insert("5302.29931640625 336.935424804688 9789.220703125".ToVector());
            pool.Insert("5984.61083984375 279.125152587891 10155.357421875".ToVector());
        }
        else if (className == "DNA_Crate_Red")
        {
            pool.Insert("1665.07763671875 452.178131103516 14395.18359375".ToVector());
            pool.Insert("1680.11022949219 448.389739990234 14211.728515625".ToVector());
            pool.Insert("1692.53210449219 452.401580810547 14173.1728515625".ToVector());
            pool.Insert("1535.07531738281 452.562866210938 14172.142578125".ToVector());
            pool.Insert("1567.43322753906 456.284240722656 14039.5283203125".ToVector());
            pool.Insert("1891.14404296875 448.797119140625 13704.33203125".ToVector());
            pool.Insert("1082.58129882812 469.209655761719 13893.326171875".ToVector());
            pool.Insert("603.0458984375 510.085723876953 13674.3955078125".ToVector());
            pool.Insert("3697.67163085938 364.566497802734 14825.0537109375".ToVector());
            pool.Insert("1390.97290039062 470.391296386719 14437.861328125".ToVector());
        }
    }
}
