modded class MissionServer
{
    protected bool m_EoH_ServerInitialized;

    override void OnInit()
    {
        super.OnInit();

        EoH_Server_Init();
    }

    protected void EoH_Server_Init()
    {
        if (m_EoH_ServerInitialized)
            return;

        Print("[EoH_Server] Initializing core systems...");

        // Load persistent world state first. Capture and AI systems depend on this existing.
        EoH_WorldStateManager.Get();

        // Load AI config before capture completions can trigger AI hooks.
        EoH_AIManager.Get();

        // Load capture config and active capture manager.
        EoH_CaptureManager.Get();

        // Main EoH server loop. CaptureManager internally respects its configured TickSeconds.
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_Server_Tick, 1000, true);

        m_EoH_ServerInitialized = true;

        Print("[EoH_Server] Core systems initialized.");
    }

    void EoH_Server_Tick()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        EoH_CaptureManager captureManager = EoH_CaptureManager.Get();
        if (captureManager)
            captureManager.Tick();
    }
};
