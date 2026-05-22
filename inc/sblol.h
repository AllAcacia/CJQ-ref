/*
 * Filename: sblol.h
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Classic meme.
 */


#ifndef SBLOL_H
#define SBLOL_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <3ds.h>
#include <citro2d.h>
#include "gamestate.h"
#include "cardCJQ.h"
#include "ctrli.h"
#include "sprCJQ.h"
#include "dynamicSS.h"
#include "rendCORE.h"
#include "auraCORE.h"
#include "audCJQ.h"


#define SPRITESHEET_PATH_SBLOL_1 "romfs:/gfx/stickbug1.t3x"
#define SPRITESHEET_PATH_SBLOL_2 "romfs:/gfx/stickbug2.t3x"
#define SPRITESHEET_PATH_SBLOL_3 "romfs:/gfx/stickbug3.t3x"


typedef struct {
    CJQ_RenderSprite sblol_spr;
    C2D_SpriteSheet sheet_1;
	C2D_SpriteSheet sheet_2;
	C2D_SpriteSheet sheet_3;
	size_t sheet_cnt_1;
	size_t sheet_cnt_2;
	size_t sheet_cnt_3;
	size_t sblol_cnt_all;
    u64 rate_ticks;
} StickBug_Sprite;


int StickBug_Launch(void);

void StickBug_Sprite_Init(StickBug_Sprite* obj);

void StickBug_Sprite_Free(StickBug_Sprite* obj);

int StickBug_Sprite_AnimHandler(StickBug_Sprite* spr_ptr);

#endif // SBLOL_H