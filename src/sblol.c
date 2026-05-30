/*
 * Filename: sblol.c
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Classic meme.
 */

#include "sblol.h"


int StickBug_Launch(void)
{
	GS_SetGameState(MODE_EXTRA);
	printf("Launching Stickbug...\n");

	RendCORE_LoadingScreen_Show(); // Show Loading Screen
    RendCORE_Execute();

	StickBug_Sprite sblol;
	StickBug_Sprite_Init(&sblol);

	GS_SetFrameTickPrv(svcGetSystemTick()); // set reference for tick
	GS_ResetFrameTickNet();

	// Set up string and textbuffer
	char* hello_str;
	CJQ_RendText hello_txt;
	u8* username = GS_GetUsername_UTF8();
	hello_str = malloc(sizeof(char)*(6 + USERNAME_LEN_MAX*UTF8_PER_UTF16_CMN));
	strcpy(hello_str, "Hello ");
	memcpy(&hello_str[6], (char*)username, sizeof(char)*USERNAME_LEN_MAX*UTF8_PER_UTF16_CMN);
	SprCJQ_RendText_Init(&hello_txt, TOP_SCREEN, hello_str, TxtBuf_STATIC, C2D_AlignCenter | C2D_AtBaseline | C2D_WithColor, NULL, (float)TOP_SCREEN_WIDTH/2, (float)TOP_SCREEN_HEIGHT/2, 1.0f, C2D_WHITE);
	printf("Prepared string [%s]\n", hello_str);
	RendCORE_TxtLyr_AppendText(&hello_txt);

	RendCORE_LoadingScreen_Hide(); // Hide Loading Screen
	VorbisFileData sblol_aud = AuraCORE_VorbisOpen(AUDIO_PATH_SBLOL);
	VorbisPlayback* aud_slot = AuraCORE_AudioAppend(&sblol_aud);
	printf("Playing Stickbug\n");

	s16 count = 1;
	printf("Count = %d\n", count);

	while (GS_GetGameState() == MODE_EXTRA && aptMainLoop()) {
		// Update tick tracking
		GS_UpdTickNow();
		u64 tick_frame = GS_GetTickFrame();
		GS_IncrFrameTickNet(tick_frame);
		GS_IncrTimeSec();

		// Read Inputs
		CTRLi_CaptureInputState();
        CTRLi_State i_state = CTRLi_GetInputState();

		if ((i_state.kDown & KEY_B)) { // exit
			GS_SetGameState(MODE_MENU);
		}

		if ((i_state.kDown & KEY_A)) { // Pause / Resume playback
			if (AuraCORE_GetIsPlaybackPaused(aud_slot)) {
				AuraCORE_ResumePlayback(aud_slot);
			} else {
				AuraCORE_PausePlayback(aud_slot);
			}
		}

		// Update animation(s)
		if (GS_GetFrameTickNet() >= GS_GetSBLOLAnimTicks()) {
			StickBug_Sprite_AnimHandler(&sblol);
			// Reset accumulated frame ticks
			GS_ResetFrameTickNet();
		}

		if (!sblol_aud.in_use) {
			AuraCORE_AudioAppend(&sblol_aud);
			count += 1;
			printf("Count = %d\n", count);
		}

		// Render the scene
		RendCORE_Execute();

		// Pass tick reference
		GS_SetFrameTickPrv(GS_GetTickNow());
	}

	RendCORE_LoadingScreen_Show(); // Show Loading Screen
	RendCORE_Execute();

	AuraCORE_StopAllPlayback();
	AuraCORE_VorbisClose(&sblol_aud);
	StickBug_Sprite_Free(&sblol);
	free(hello_str);
	RendCORE_TxtLyr_DeleteText(&hello_txt);

	printf("Exiting Stickbug...\n");

	return EXIT_SUCCESS;
}


void StickBug_Sprite_Init(StickBug_Sprite* obj)
{
	obj->sheet_1 = C2D_SpriteSheetLoad(SPRITESHEET_PATH_SBLOL_1);
	obj->sheet_2 = C2D_SpriteSheetLoad(SPRITESHEET_PATH_SBLOL_2);
	obj->sheet_3 = C2D_SpriteSheetLoad(SPRITESHEET_PATH_SBLOL_3);
	
	obj->sheet_cnt_1 = C2D_SpriteSheetCount(obj->sheet_1);
	obj->sheet_cnt_2 = C2D_SpriteSheetCount(obj->sheet_2);
	obj->sheet_cnt_3 = C2D_SpriteSheetCount(obj->sheet_3);

	obj->sblol_cnt_all = obj->sheet_cnt_1 + obj->sheet_cnt_2 + obj->sheet_cnt_3;

	obj->rate_ticks = GS_GetTickDelay_hz(SBLOL_RATE_HZ);
	
	SprCJQ_RendSprite_Init(&(obj->sblol_spr), BOT_SCREEN, RendLayer_BACKGND, 0);
	C2D_SpriteFromSheet(&(obj->sblol_spr.c2d_spr), obj->sheet_1, 0); // take first frame
	RendCORE_SprLyr_AppendSprite(&(obj->sblol_spr));
}


void StickBug_Sprite_Free(StickBug_Sprite* obj)
{
	C2D_SpriteSheetFree(obj->sheet_1);
    C2D_SpriteSheetFree(obj->sheet_2);
	C2D_SpriteSheetFree(obj->sheet_3);
	RendCORE_SprLyr_DeleteSprite(&(obj->sblol_spr));
}


int StickBug_Sprite_AnimHandler(StickBug_Sprite* spr_ptr)
{
	int result = 0;
	static size_t sblol_index = 0;
	// Run animation simul
	sblol_index = (sblol_index + 1) % spr_ptr->sblol_cnt_all;
	result = 1;

	size_t sheet_index = 0;

	if (sblol_index < spr_ptr->sheet_cnt_1) {
		sheet_index = sblol_index;
		C2D_SpriteFromSheet(&(spr_ptr->sblol_spr.c2d_spr), spr_ptr->sheet_1, sheet_index);
	} else if (sblol_index < (spr_ptr->sheet_cnt_1 + spr_ptr->sheet_cnt_2)) {
		sheet_index = sblol_index % (spr_ptr->sheet_cnt_1);
		C2D_SpriteFromSheet(&(spr_ptr->sblol_spr.c2d_spr), spr_ptr->sheet_2, sheet_index);
	} else if (sblol_index < (spr_ptr->sheet_cnt_1 + spr_ptr->sheet_cnt_2 + spr_ptr->sheet_cnt_3)) {
		sheet_index = sblol_index % (spr_ptr->sheet_cnt_1 + spr_ptr->sheet_cnt_2);
		C2D_SpriteFromSheet(&(spr_ptr->sblol_spr.c2d_spr), spr_ptr->sheet_3, sheet_index);
	}

	return result;
}