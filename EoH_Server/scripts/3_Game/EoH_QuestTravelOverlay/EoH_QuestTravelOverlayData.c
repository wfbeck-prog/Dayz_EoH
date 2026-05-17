class EoH_QuestTravelOverlayData
{
    int QuestID;
    int ObjectiveID;
    string Label;
    vector TruePosition;
    vector CircleCenter;
    float Radius;

    void EoH_QuestTravelOverlayData(int questId = 0, int objectiveId = 0, string label = "", vector truePosition = "0 0 0", float radius = 350.0)
    {
        QuestID = questId;
        ObjectiveID = objectiveId;
        Label = label;
        TruePosition = truePosition;
        Radius = radius;
        CircleCenter = ComputeOffsetCenter(truePosition, questId, objectiveId, radius);
    }

    string GetId()
    {
        return "EoH_TRAVEL_" + QuestID.ToString() + "_" + ObjectiveID.ToString();
    }

    static vector ComputeOffsetCenter(vector truePosition, int questId, int objectiveId, float radius)
    {
        // Deterministic offset: target stays inside circle, but is not centered.
        int seed = (questId * 37) + (objectiveId * 17);
        float angleDeg = Math.Mod(seed, 360);
        float angleRad = angleDeg * Math.DEG2RAD;
        float distance = Math.Clamp(radius * 0.45, 75.0, radius * 0.70);

        vector center = truePosition;
        center[0] = truePosition[0] + (Math.Cos(angleRad) * distance);
        center[2] = truePosition[2] + (Math.Sin(angleRad) * distance);
        return center;
    }
};
