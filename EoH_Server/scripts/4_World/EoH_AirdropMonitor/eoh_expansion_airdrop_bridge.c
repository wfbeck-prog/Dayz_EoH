/*
    EoH Expansion Airdrop Bridge - DISABLED

    Directly modding ExpansionMissionEventAirdrop causes the installed Expansion build to fail at:

        DayZExpansion/Missions/Scripts/4_World/dayzexpansion_missions/classes/airdrop/expansionmissioneventairdrop.c(298): Incompatible parameter 'this'

    Root cause:
    Expansion loads JSON into `this` with:

        ExpansionJsonFileParser<ExpansionMissionEventAirdrop>.Load(m_FileName, this);

    When another mod extends the same class with `modded class ExpansionMissionEventAirdrop`, that generic parser call can reject the modified runtime type.

    Do not re-enable this direct class override.

    Safe options left:
    1. Use Expansion's built-in SocialMediaSettings/notification/webhook support if available.
    2. Add a separate EoH poller that watches server logs for `[MissionAirdrop]` lines.
    3. Add explicit calls from a forked/custom copy of ExpansionMissionEventAirdrop.c, if you are willing to maintain that patch.

    The webhook-side router is still valid:

        EoH_AirdropMissionMonitor.Get().OnExpansionAirdropMissionStarted(missionName, locationName, position);
        EoH_AirdropMissionMonitor.Get().OnExpansionAirdropLanded(missionName, locationName, position);
*/
