// trimmed for brevity - only modified sections

// inside Tick(), replace progress print block with UI
if (CanProgress(session, townCfg))
{
    session.Progress += delta;

    float percent = (session.Progress / (townCfg.CaptureTimeSeconds * 1000.0)) * 100.0;
    string msg = "Capturing " + session.TownName + ": " + Math.Round(percent).ToString() + "%";

    EoH_CaptureUI.Broadcast(msg);
}
else
{
    if (session.IsContested)
        EoH_CaptureUI.Broadcast("⚠ " + session.TownName + " is CONTESTED");
}

// inside CompleteCapture()
void CompleteCapture(EoH_CaptureSession session)
{
    EoH_WorldStateManager.Get().SetTownOwner(session.TownName, session.AttackingGroupID, session.AttackingGroupName);

    EoH_CaptureUI.Broadcast("🏴 " + session.TownName + " captured by " + session.AttackingGroupName);

    Print("[EoH_Capture] Capture complete: " + session.TownName);

    m_ActiveCaptures.Remove(session.TownName);
}
