modded class MissionGameplay
{
    override void OnEvent(EventType eventTypeId, Param params)
    {
        super.OnEvent(eventTypeId, params);

        if (eventTypeId != ChatMessageEventTypeID && eventTypeId != MPSessionPlayerReadyEventTypeID)
        {
            // Keep this hook lightweight. Field report UI opens through direct RPC queue below when available.
        }
    }

    void EoH_OpenFieldReport(EoH_FieldReportData report)
    {
        if (!report)
            return;

        EoH_FieldReportMenu.Open(report);
    }
};
