// SNIPPET ONLY: Inject AI hook into SetTownOwner

    void SetTownOwner(string townName, string groupID, string groupName)
    {
        EoH_WorldStateTownState town = GetTownState(townName);

        if (!town)
        {
            town = new EoH_WorldStateTownState();
            town.Name = townName;
            town.Tier = 1;
            m_State.Towns.Insert(town);
        }

        int now = GetGame().GetTime();

        town.OwnerGroupID = groupID;
        town.OwnerGroupName = groupName;
        town.Influence = 100.0;
        town.CapturedAtUnix = now;
        town.LastChangedUnix = now;

        Print("[EoH_WorldState] Town owner changed: " + townName + " -> " + groupName);

        // NEW AI HOOK
        EoH_AIManager.Get().OnTownCaptured(townName, town.Tier);

        SaveState();
    }
