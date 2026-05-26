class EoH_LiveAdvisorEvent
{
    string Type;
    string Timestamp;
    string Severity;
    string Message;
    string Source;

    void EoH_LiveAdvisorEvent(string type = "INFO", string message = "", string severity = "info", string source = "EoH_Server")
    {
        Type = type;
        Message = message;
        Severity = severity;
        Source = source;
        Timestamp = "";
    }
}
