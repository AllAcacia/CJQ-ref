/*
 * Filename: rendCORE.h
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Handles lower-level sprite
 * rendering by categorizing
 * sprites into different scene
 * layers, and supports draw
 * ordering independent of game-
 * play.
 */

#ifndef RENDCORE_H
#define RENDCORE_H

#include <stdint.h>
#include "sprCJQ.h"
#include "gamestate.h"

typedef struct {
    C2D_SpriteSheet menu_load_sheet;
    CJQ_RenderSprite menu_load_top_spr;
    CJQ_RenderSprite menu_load_bot_spr;
} CJQ_LoadMenuAssets;

void RendCORE_Init(void);

void RendCORE_Exit(void);

void RendCORE_Execute(void);

void RendCORE_Execute_Screen(CJQ_LayerSprites* lyr_collection);

void RendCORE_SprLyrCollection_Init(CJQ_LayerSprites collection[NUM_SCREENS][RendLayer_COUNT]);

int RendCORE_SprLyr_AppendSprite(CJQ_RenderSprite* rend_sprite_ptr);

int RendCORE_SprLyr_DeleteSprite(CJQ_RenderSprite* rend_sprite_ptr);

int16_t RendCORE_FindSprPtr(CJQ_RenderSprite** arr, CJQ_RenderSprite* ptr, uint16_t arr_len);

int RendCORE_SprLyr_SortSprites(CJQ_Screen screen, CJQ_RendLayer lyr_i);

int RendCORE_spr_order_cmp(const CJQ_RenderSprite* spr_a_ptr, const CJQ_RenderSprite* spr_b_ptr);

void RendCORE_LoadingScreen_Show(void);

void RendCORE_LoadingScreen_Hide(void);

#endif // RENDCORE_H