// EoH_RoamingTrader - Punched Card Patch
// Purpose:
// - Adds the bunker/underground panel use action back onto PunchedCard.
// - Damages the punched card after use.
//
// Future EoH_Server merge note:
// - This file can be moved directly into EoH_Server/scripts/4_World/ later.
// - Keep only one modded PunchedCard class active across your final combined server mod.

modded class PunchedCard : Paper
{
	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionUseUndergroundPanel);
	}

	override void OnUse()
	{
		AddHealthLevel(4);
	}
}
