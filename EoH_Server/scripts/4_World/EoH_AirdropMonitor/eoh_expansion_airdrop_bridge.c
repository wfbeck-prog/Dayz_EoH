/*
    EoH Expansion Airdrop Bridge

    Disabled intentionally.

    The installed Expansion Missions build rejects directly modding ExpansionMissionEventAirdrop
    with this bridge and throws:

        DayZExpansion/Missions/Scripts/4_World/dayzexpansion_missions/classes/airdrop/expansionmissioneventairdrop.c(298): Incompatible parameter 'this'

    Keep this file as documentation only until the exact Expansion callback signature is matched.
    The safe webhook-side implementation remains in:

        EoH_Server/scripts/4_World/EoH_AirdropMonitor/eoh_airdrop_mission_monitor.c

    Once the correct Expansion event hook is confirmed, call:

        EoH_AirdropMissionMonitor.Get().OnExpansionAirdropMissionStarted(missionName, locationName, position);
        EoH_AirdropMissionMonitor.Get().OnExpansionAirdropLanded(missionName, locationName, position);
*/
