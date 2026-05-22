/*
 * Filename: game.h
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Highest level game module.
 */

#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <3ds.h>
#include <citro2d.h>

#include "proto.h"
#include "pyro.h"
#include "hydro.h"
#include "cryo.h"
#include "sblol.h"

#include "debug.h"
#include "gamestate.h"
#include "cardCJQ.h"
#include "ctrli.h"
#include "sprCJQ.h"
#include "dynamicSS.h"
#include "rendCORE.h"
#include "auraCORE.h"


typedef struct {
    C2D_SpriteSheet menu_main_sheet;
    CJQ_RenderSprite menu_main_top_spr;
    CJQ_RenderSprite menu_main_bot_spr;
} CJQ_MainMenuAssets;


void CJQ_NavigateMenu(CJQ_MainMenuAssets* assets);

void CJQ_MainMenuAssets_Init(CJQ_MainMenuAssets* obj);

void CJQ_MainMenuAssets_Free(CJQ_MainMenuAssets* obj);

void CJQ_MainMenuAssets_Show(CJQ_MainMenuAssets* obj);

void CJQ_MainMenuAssets_Hide(CJQ_MainMenuAssets* obj);

#endif // GAME_H