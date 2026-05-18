class EoH_QuestTravelOverlayClient
{
    protected static ref map<string, ref EoH_QuestCircleData> s_TravelCircles = new map<string, ref EoH_QuestCircleData>();

    static void AddCircle(EoH_QuestCircleData circle)
    {
        if (!circle)
            return;

        circle.Normalize();
        s_TravelCircles.Set(circle.Id, circle);
        Print("[EoH_TravelOverlayClient] Added circle id=" + circle.Id + " label=" + circle.Label + " pos=" + circle.Position.ToString() + " radius=" + circle.Radius.ToString());
    }

    static void RemoveCircle(string id)
    {
        if (id == "")
            return;

        if (s_TravelCircles.Contains(id))
            s_TravelCircles.Remove(id);

        Print("[EoH_TravelOverlayClient] Removed circle id=" + id);
    }

    static map<string, ref EoH_QuestCircleData> GetCircles()
    {
        return s_TravelCircles;
    }
};

// NOTE:
// MissionGameplay has no OnRPC override in this script context.
// The old override was removed to fix compile errors.
// Travel overlay circle rendering/storage remains available through EoH_QuestTravelOverlayClient,
// but RPC receive + map rendering still need a safe CF/Event-based implementation pass.
