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

        string playerName = "UNKNOWN";
        string playerId = "UNKNOWN";

        if (player.GetIdentity())
        {
            playerName = player.GetIdentity().GetName();
            playerId = player.GetIdentity().GetId();
        }

        if (m_Config.EnableDebug)
            Print("[EoH_SCB] Scheduling secure container restore for player=" + playerName + " steamId=" + playerId + " delayMs=" + m_Config.RestoreDelayMs.ToString());

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RestoreSecureContainerAttempt, m_Config.RestoreDelayMs, false, player, 0);
    }

    void RestoreSecureContainerAttempt(PlayerBase player, int attempt)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!player || !player.IsAlive() || !m_Config || !m_Config.PersistOnDeath)
            return;

        string playerName = "UNKNOWN";
        string playerId = "UNKNOWN";

        if (player.GetIdentity())
        {
            playerName = player.GetIdentity().GetName();
            playerId = player.GetIdentity().GetId();
        }

        EntityAI existingContainer = player.GetSecureContainer();
        if (existingContainer)
        {
            if (m_Config.EnableDebug)
                Print("[EoH_SCB] Existing A6 secure container detected for player=" + playerName + " type=" + existingContainer.GetType() + " attempt=" + attempt.ToString() + " - bridge will NOT replace it.");

            return;
        }

        string containerClass = EoH_SCB_Skill.GetContainerClassForPlayer(player, m_Config);
        if (containerClass == "")
        {
            if (m_Config.EnableDebug)
                Print("[EoH_SCB][WARN] No valid secure container class resolved for player=" + playerName + " attempt=" + attempt.ToString());

            return;
        }

        EntityAI createdContainer = player.GetInventory().CreateInInventory(containerClass);
        if (createdContainer)
        {
            Print("[EoH_SCB] Restored secure container for player=" + playerName + " steamId=" + playerId + " class=" + containerClass + " attempt=" + attempt.ToString());
            return;
        }

        Print("[EoH_SCB][WARN] Failed to create secure container class=" + containerClass + " player=" + playerName + " attempt=" + attempt.ToString());

        if (attempt < m_Config.RestoreRetryCount)
        {
            int nextAttempt = attempt + 1;

            Print("[EoH_SCB] Scheduling retry attempt=" + nextAttempt.ToString() + " player=" + playerName + " retryDelayMs=" + m_Config.RestoreRetryDelayMs.ToString());

            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RestoreSecureContainerAttempt, m_Config.RestoreRetryDelayMs, false, player, nextAttempt);
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
