class CfgPatches
{
	class EoH_MenuTheme
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 1.0;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Sounds_Effects"
		};
	};
};

class CfgMods
{
	class EoH_MenuTheme
	{
		dir = "EoH_MenuTheme";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "EoH Menu Theme";
		credits = "EoH";
		author = "EoH";
		authorID = "0";
		version = "1.0";
		type = "mod";
		dependencies[] = { "Game" };

		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = { "EoH_MenuTheme/scripts/3_Game" };
			};
		};
	};
};

class CfgSoundShaders
{
	class EoH_MenuMusic_Shader
	{
		samples[] = {{"EoH_MenuTheme\sounds\loading.ogg",1}};
		volume = 0.5;
		range = 10;
	};
};

class CfgSoundSets
{
	class EoH_MenuMusic_SoundSet
	{
		soundShaders[] = {"EoH_MenuMusic_Shader"};
		volumeFactor = 1.0;
		frequencyRandomizer = 0;
		spatial = 0;
		doppler = 0;
		loop = 1;
		sound3DProcessingType = "none";
	};
};