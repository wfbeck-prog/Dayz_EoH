EoH_MenuTheme
=============

This pack gives you:
1. Custom loading screen background path
2. Custom main menu/loading music using loading.ogg
3. Corrected mod prefix and script paths

IMPORTANT:
- You still need to add your image as: EoH_MenuTheme/data/menu_bg.edds
- Build the .edds in DayZ Workbench from your PNG/JPG.
- Recommended image size: 2560x1440

FOLDER STRUCTURE
EoH_MenuTheme/
  config.cpp
  pbo.json
  data/menu_bg.edds        <-- add this yourself
  sounds/loading.ogg
  scripts/3_Game/ModdedDayZGame.c

BUILD
- Open Addon Builder
- Pack the EoH_MenuTheme folder
- Output the PBO to @EoH_MenuTheme/Addons/
- Sign with DSUtils if you want workshop/server distribution

NOTES
- This replaces the loading screen image path and queue screen background path.
- Music is started when the main menu/loading UI is shown.
- If another loading screen/main menu mod also overrides LoadingScreen, they can conflict.
