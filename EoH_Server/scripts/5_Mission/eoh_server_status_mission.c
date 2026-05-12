modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EoH_ServerStatusWebhook.NotifyOnline, 30000, false);
    }
};
