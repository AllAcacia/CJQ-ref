/*
 * Filename: game.c
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Highest level game module.
 */

#include "game.h"


int main(int argc, char *argv[])
{
	// Initialise console libs
	romfsInit();
	gfxInitDefault();
	hidInit();
	
	// Initialise C2D libs
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// Debug module init
	Debug_Init();    // Connection via 3dslink

	// Load Card Database
	Cards_LoadDatabase();

	// Initialize CJQ module libs
	GS_Init();       // General game_state stuff
	RendCORE_Init(); // Sprite + scene layer manager
	AuraCORE_Init(); // Audio manager
	NetCORE_Init();  // Network manager

	printf("Initialised all modules for %s\n", (char*)GS_GetUsername_UTF8());

	CJQ_MainMenuAssets main_menu_assets;
	CJQ_MainMenuAssets_Init(&main_menu_assets);

	// Main loop
	while (aptMainLoop())
	{
		// Scan all the inputs. This should be done once for each frame
		CTRLi_CaptureInputState();
		CTRLi_State i_state = CTRLi_GetInputState();

		if ((i_state.kDown & KEY_START) && (GS_GetGameState() == MODE_MENU)) { // exit
			break; // break in order to return to hbmenu
		} else {
			CJQ_NavigateMenu(&main_menu_assets);
		}
		RendCORE_Execute();
	}

	Cards_CloseDatabase();

	// Unload Main-Menu sprites
	CJQ_MainMenuAssets_Free(&main_menu_assets);

	printf("Attempting to exit all modules...\n");
	
	// Unload all modules and libraries
	NetCORE_Exit();
	AuraCORE_Exit();
	RendCORE_Exit();
	GS_Exit();
	C2D_Fini();
	C3D_Fini();
	hidExit();
	gfxExit();
	romfsExit();

	printf("Exited all modules\n");
	printf("Closing...\n");

	return 0;
}


void CJQ_NavigateMenu(CJQ_MainMenuAssets* assets)
{
    GS_IncrTimeSec();
	CTRLi_State i_state = CTRLi_GetInputState();

	// navigate between game titles
	if ((GS_GetGameState() != MODE_PROTO) && (i_state.kDown & PROTO_SELECT)) { // CJ base
		printf("Launching Proto...\n");
		CJQ_MainMenuAssets_Hide(assets);
        ProtoCJQ_Launch();
	} else if ((GS_GetGameState() != MODE_PYRO) && (i_state.kDown & PYRO_SELECT)) { // CJ fire
		printf("Launching Pyro...\n");
		CJQ_MainMenuAssets_Hide(assets);
        PyroCJQ_Launch();
	} else if ((GS_GetGameState() != MODE_HYDRO) && (i_state.kDown & HYDRO_SELECT)) { // CJ water
		printf("Launching Hydro...\n");
		CJQ_MainMenuAssets_Hide(assets);
        HydroCJQ_Launch();
	} else if ((GS_GetGameState() != MODE_CRYO) && (i_state.kDown & CRYO_SELECT)) { // CJ snow
		printf("Launching Cryo...\n");
		CJQ_MainMenuAssets_Hide(assets);
        CryoCJQ_Launch();
	}

	if (GS_GetGameState() == MODE_MENU) {
		RendCORE_LoadingScreen_Hide();
		CJQ_MainMenuAssets_Show(assets);
	}
}


void CJQ_MainMenuAssets_Init(CJQ_MainMenuAssets* obj)
{
	// Load Main-Menu sprites
	obj->menu_main_sheet = C2D_SpriteSheetLoad(SPRITESHEET_PATH_MAIN_MENU);
	// Top Main-Menu Screen
	C2D_SpriteSetCenter(&obj->menu_main_top_spr.c2d_spr, 0.5f, 0.5f);
	C2D_SpriteSetPos(&obj->menu_main_top_spr.c2d_spr, TOP_SCREEN_WIDTH/2, TOP_SCREEN_HEIGHT/2);
	SprCJQ_RendSprite_Init(&obj->menu_main_top_spr, TOP_SCREEN, RendLayer_BACKGND, INT16_MIN);
	C2D_SpriteFromSheet(&obj->menu_main_top_spr.c2d_spr, obj->menu_main_sheet, 0);
	RendCORE_SprLyr_AppendSprite(&obj->menu_main_top_spr);
	// Bottom Main-Menu Screen
	C2D_SpriteSetCenter(&obj->menu_main_bot_spr.c2d_spr, 0.5f, 0.5f);
	C2D_SpriteSetPos(&obj->menu_main_bot_spr.c2d_spr, BOTTOM_SCREEN_WIDTH/2, BOTTOM_SCREEN_HEIGHT/2);
	SprCJQ_RendSprite_Init(&obj->menu_main_bot_spr, BOT_SCREEN, RendLayer_BACKGND, INT16_MIN);
	C2D_SpriteFromSheet(&obj->menu_main_bot_spr.c2d_spr, obj->menu_main_sheet, 1);
	RendCORE_SprLyr_AppendSprite(&obj->menu_main_bot_spr);
}


void CJQ_MainMenuAssets_Free(CJQ_MainMenuAssets* obj)
{
	RendCORE_SprLyr_DeleteSprite(&obj->menu_main_top_spr);
	RendCORE_SprLyr_DeleteSprite(&obj->menu_main_bot_spr);
	C2D_SpriteSheetFree(obj->menu_main_sheet);
}


void CJQ_MainMenuAssets_Show(CJQ_MainMenuAssets* obj)
{
	SprCJQ_RendSprite_Show(&obj->menu_main_top_spr);
	SprCJQ_RendSprite_Show(&obj->menu_main_bot_spr);
}


void CJQ_MainMenuAssets_Hide(CJQ_MainMenuAssets* obj)
{
	SprCJQ_RendSprite_Hide(&obj->menu_main_top_spr);
	SprCJQ_RendSprite_Hide(&obj->menu_main_bot_spr);
}