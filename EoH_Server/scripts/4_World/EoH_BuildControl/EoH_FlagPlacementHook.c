modded class ActionDeployObject
{
    override void OnFinishProgressServer(ActionData action_data)
    {
        super.OnFinishProgressServer(action_data);

        if (!action_data || !action_data.m_MainItem || !action_data.m_Player)
            return;

        string type = action_data.m_MainItem.GetType();

        if (type != "TerritoryFlagKit")
            return;

        vector pos = action_data.m_Player.GetPosition();

        array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition(pos, 5.0, objects, null);

        foreach (Object obj : objects)
        {
            Flag_Base flag = Flag_Base.Cast(obj);
            if (!flag)
                continue;

            // Avoid overwriting if already set
            if (flag.GetEoHOwner() != "")
                continue;

            string groupID = EoH_GroupHelper.GetGroupID(action_data.m_Player);
            if (groupID == "")
                return;

            flag.SetEoHOwner(groupID);

            action_data.m_Player.MessageStatus("Territory claimed for your group.");
            break;
        }
    }
};
