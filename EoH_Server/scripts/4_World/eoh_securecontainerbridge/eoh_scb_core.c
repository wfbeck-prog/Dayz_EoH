class EoH_SCB_Core
{
    protected static ref EoH_SCB_Core s_Instance;
    protected ref EoH_SCB_Config m_Config;

    void EoH_SCB_Core()
    {
        m_Config = new EoH_SCB_Config();
    }

    static EoH_SCB_Core Get()
    {
        if (!s_Instance)
            s_Instance = new EoH_SCB_Core();

        return s_Instance;
    }

    EoH_SCB_Config GetConfig()
    {
        return m_Config;
    }

    void ScheduleRestore(PlayerBase player)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!player || !m_Config || !m_Config.PersistOnDeath)
            return;

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RestoreSecureContainer, m_Config.RestoreDelayMs, false, player);
    }

    void RestoreSecureContainer(PlayerBase player)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!player || !player.IsAlive() || !m_Config || !m_Config.PersistOnDeath)
            return;

        string containerClass = EoH_SCB_Skill.GetContainerClassForPlayer(player, m_Config);
        if (containerClass == "")
            return;

        EntityAI existingContainer = player.GetSecureContainer();
        if (existingContainer)
        {
            if (m_Config.EnableDebug)
                Print("[EoH_SCB] Player already has secure container: " + existingContainer.GetType());

            return;
        }

        EntityAI createdContainer = player.GetInventory().CreateInInventory(containerClass);
        if (createdContainer)
        {
            if (m_Config.EnableDebug)
                Print("[EoH_SCB] Restored secure container: " + containerClass);
        }
        else
        {
            Print("[EoH_SCB][WARN] Failed to create secure container: " + containerClass);
        }
    }
};

modded class PlayerBase
{
    override void OnTerjePlayerRespawned()
    {
        super.OnTerjePlayerRespawned();

        if (GetGame() && GetGame().IsServer())
            EoH_SCB_Core.Get().ScheduleRestore(this);
    }
};
