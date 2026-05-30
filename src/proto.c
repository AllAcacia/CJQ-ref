/*
 * Filename: proto.c
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Overarching module controlling
 * the original Card-Jitsu game!
 */


#include "proto.h"


int ProtoCJQ_Launch(void)
{
    GS_SetGameState(MODE_PROTO);

    RendCORE_LoadingScreen_Show(); // Show Loading Screen
    RendCORE_Execute();

    // Load Card Assets
    CJQ_RendSprite card;
    uint8_t element_i = CARD_FIRE;
    uint8_t rank_i = CARD_RANK_MIN;
    float card_x = (float)BOTTOM_SCREEN_WIDTH/2;
    float card_y = (float)BOTTOM_SCREEN_HEIGHT/2;
    SprCJQ_RendSprite_Init(&card, BOT_SCREEN, RendLayer_FRNTGND, 0);
    C2D_SpriteSheet cards_fire_sheet = C2D_SpriteSheetLoad(SPRITESHEET_PATH_CARDS_FIRE);
    C2D_SpriteSheet cards_water_sheet = C2D_SpriteSheetLoad(SPRITESHEET_PATH_CARDS_WATER);
    C2D_SpriteSheet cards_snow_sheet = C2D_SpriteSheetLoad(SPRITESHEET_PATH_CARDS_SNOW);
    C2D_SpriteFromSheet(&card.c2d_spr, cards_fire_sheet, 0);
    C2D_SpriteSetCenter(&card.c2d_spr, 0.5f, 0.5f);
    C2D_SpriteSetPos(&card.c2d_spr, card_x, card_y);
    RendCORE_SprLyr_AppendSprite(&card);

    SecondOrderDTS touchLocX_DTS;
    SecondOrderDTS touchLocY_DTS;
    float dyn_fn = 1.0f;
    float dyn_xi = 0.75f;
    float dyn_dt = 1.0f/((float)SIMUL_RATE_HZ);
    DynamicSS_init(&touchLocX_DTS, dyn_fn, dyn_xi, dyn_dt);
    DynamicSS_setstate(&touchLocX_DTS, BOTTOM_SCREEN_WIDTH/2, 0.0f);
    DynamicSS_init(&touchLocY_DTS, dyn_fn, dyn_xi, dyn_dt);
    DynamicSS_setstate(&touchLocY_DTS, BOTTOM_SCREEN_HEIGHT/2, 0.0f);

	GS_ResetFrameTickNet();

    RendCORE_LoadingScreen_Hide(); // Hide Loading Screen

    while (GS_GetGameState() == MODE_PROTO && aptMainLoop()) {
        // Update tick tracking
		GS_UpdTickNow();
		u64 tick_frame = GS_GetTickFrame();
		GS_IncrFrameTickNet(tick_frame);
		GS_IncrTimeSec();

        // Read Inputs
		CTRLi_CaptureInputState();
        CTRLi_State i_state = CTRLi_GetInputState();
        GS_IncrTimeSec();

        if (i_state.kDown & KEY_B) {
            GS_SetGameState(MODE_MENU);
        }
        
        // Run game
		if (Cards_ScrollCards(&element_i, &rank_i)) {
            switch (element_i) {
                case CARD_FIRE:
                    C2D_SpriteFromSheet(&card.c2d_spr, cards_fire_sheet, rank_i);
                    break;
                case CARD_WATER:
                    C2D_SpriteFromSheet(&card.c2d_spr, cards_water_sheet, rank_i);
                    break;
                case CARD_SNOW:
                    C2D_SpriteFromSheet(&card.c2d_spr, cards_snow_sheet, rank_i);
                    break;
            }
            C2D_SpriteSetCenter(&card.c2d_spr, 0.5f, 0.5f);
            C2D_SpriteSetPos(&card.c2d_spr, card_x, card_y);
        }
		
		uint8_t simul_steps = 0;
		while (GS_GetFrameTickNet() >= GS_GetSimulTicks() && simul_steps < MAX_SIMUL_STEPS) { // For time-sensitive actions
			if(i_state.kHeld & KEY_TOUCH) { // If touch-screen pressed, iterate DTS (at a preset constant rate)
				DynamicSS_iterate(&touchLocX_DTS, ((float)i_state.vtpad.px)*FLT_TO_TPADX);
				DynamicSS_iterate(&touchLocY_DTS, ((float)i_state.vtpad.py)*FLT_TO_TPADY);
			} else {
				DynamicSS_setstate(&touchLocX_DTS, card.c2d_spr.params.pos.x, 0.0f);
				DynamicSS_setstate(&touchLocY_DTS, card.c2d_spr.params.pos.y, 0.0f);
			}
			GS_ResetFrameTickNet();
			simul_steps += 1;
		}
        card_x = Mat2Dfloat_return(&touchLocX_DTS.x1, 0, 0);
        card_y = Mat2Dfloat_return(&touchLocY_DTS.x1, 0, 0);
		C2D_SpriteSetPos(&card.c2d_spr, card_x, card_y);

        RendCORE_Execute();
    }

    RendCORE_SprLyr_DeleteSprite(&card);
    C2D_SpriteSheetFree(cards_fire_sheet);
    C2D_SpriteSheetFree(cards_water_sheet);
    C2D_SpriteSheetFree(cards_snow_sheet);

    RendCORE_LoadingScreen_Show(); // Show Loading Screen

    return EXIT_SUCCESS;
}