static ref EffectSound g_EOH_MenuMusic;

modded class LoadingScreen
{
	void LoadingScreen(DayZGame game)
	{
		Class.CastTo(m_ImageLogoMid, m_WidgetRoot.FindAnyWidget("ImageLogoMid"));
		Class.CastTo(m_ImageLogoCorner, m_WidgetRoot.FindAnyWidget("ImageLogoCorner"));

		if (m_ImageLogoMid)
			m_ImageLogoMid.Show(false);

		if (m_ImageLogoCorner)
			m_ImageLogoCorner.Show(false);

		if (m_ImageBackground)
		{
			m_ImageBackground.LoadImageFile(0, "EoH_MenuTheme/data/menu_bg.edds");
			m_ImageBackground.LoadMaskTexture("");
			ProgressAsync.SetUserData(m_ImageBackground);
		}
	}

	override void Show()
	{
		Widget lIcon = m_ImageBackground;
		Widget pText = m_ProgressLoading;

		m_ProgressText.SetText("");
		m_ProgressLoading.SetCurrent(0.0);

		if (m_ImageBackground)
			m_ImageBackground.SetMaskProgress(0.0);

		if (!m_WidgetRoot.IsVisible())
		{
			if (m_DayZGame.GetUIManager().IsDialogVisible())
			{
				m_DayZGame.GetUIManager().HideDialog();
			}

			if (m_DayZGame.GetMissionState() == DayZGame.MISSION_STATE_MAINMENU)
			{
				m_ImageWidgetBackground.Show(true);
				m_TextWidgetStatus.Show(true);
			}
			else
			{
				m_ImageWidgetBackground.Show(true);
				m_TextWidgetStatus.Show(false);
			}

			m_WidgetRoot.Show(true);
			m_TextWidgetTitle.SetText("");
			m_TextWidgetStatus.SetText("");
		}

		// force menu music whenever main menu is active
		if (m_DayZGame.GetMissionState() == DayZGame.MISSION_STATE_MAINMENU)
		{
			PlayEoHMenuMusic();
		}
		else
		{
			StopEoHMenuMusic();
		}

		ProgressAsync.SetProgressData(pText);
		ProgressAsync.SetUserData(lIcon);
	}

	protected void PlayEoHMenuMusic()
	{
		if (!GetGame() || GetGame().IsDedicatedServer())
			return;

		if (g_EOH_MenuMusic && g_EOH_MenuMusic.IsSoundPlaying())
			return;

		StopEoHMenuMusic();

		g_EOH_MenuMusic = SEffectManager.CreateSound("EoH_MenuMusic_SoundSet", vector.Zero, 0, 0, false);

		if (g_EOH_MenuMusic)
		{
			g_EOH_MenuMusic.SetSoundLoop(true);
			g_EOH_MenuMusic.SetSoundAutodestroy(false);
			g_EOH_MenuMusic.SetSoundVolume(1.0);
			g_EOH_MenuMusic.SoundPlay();
		}
	}

	protected void StopEoHMenuMusic()
	{
		if (g_EOH_MenuMusic)
		{
			g_EOH_MenuMusic.SetSoundFadeOut(0.5);
			g_EOH_MenuMusic.SoundStop();
			g_EOH_MenuMusic = null;
		}
	}
}

modded class LoginQueueBase
{
	override Widget Init()
	{
		Widget w = super.Init();
		ImageWidget imageWidgetBackground = ImageWidget.Cast(layoutRoot.FindAnyWidget("Background"));

		if (imageWidgetBackground)
		{
			imageWidgetBackground.LoadMaskTexture("");
			imageWidgetBackground.LoadImageFile(0, "EoH_MenuTheme/data/menu_bg.edds");
		}

		return w;
	}
}

modded class LoginTimeBase
{
	override Widget Init()
	{
		Widget w = super.Init();
		ImageWidget imageWidgetBackground = ImageWidget.Cast(layoutRoot.FindAnyWidget("Background"));

		if (imageWidgetBackground)
		{
			imageWidgetBackground.LoadMaskTexture("");
			imageWidgetBackground.LoadImageFile(0, "EoH_MenuTheme/data/menu_bg.edds");
		}

		return w;
	}
}