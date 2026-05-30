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
#include <stdlib.h>
#include <citro2d.h>
#include "sprCJQ.h"
#include "gamestate.h"

#define BUF_GLYPHS_MAX 4096

typedef struct {
    C2D_SpriteSheet menu_load_sheet;
    CJQ_RendSprite menu_load_top_spr;
    CJQ_RendSprite menu_load_bot_spr;
} CJQ_LoadMenuAssets;

void RendCORE_Init(void);

void RendCORE_Exit(void);

void RendCORE_Execute(void);

void RendCORE_Execute_Screen_Spr(CJQ_SpriteLayer* lyr_collection);

void RendCORE_Execute_Screen_Txt(CJQ_TextLayer* lyr_obj);

void RendCORE_SprLyrCollection_Init(CJQ_SpriteLayer collection[NUM_SCREENS][RendLayer_COUNT]);

void RendCORE_TxtLyrCollection_Init(CJQ_TextLayer collection[NUM_SCREENS][TxtBuf_COUNT]);

int RendCORE_SprLyr_AppendSprite(CJQ_RendSprite* rend_sprite_ptr);

int RendCORE_TxtLyr_AppendText(CJQ_RendText* rend_text_ptr);

int RendCORE_SprLyr_DeleteSprite(CJQ_RendSprite* rend_sprite_ptr);

int RendCORE_TxtLyr_DeleteText(CJQ_RendText* rend_text_ptr);

int16_t RendCORE_FindSprPtr(CJQ_RendSprite** arr, CJQ_RendSprite* ptr, uint16_t arr_len);

int16_t RendCORE_FindTxtPtr(CJQ_RendText** arr, CJQ_RendText* ptr, uint16_t arr_len);

int RendCORE_SprLyr_Sort(CJQ_Screen screen, CJQ_RendLayer lyr_i);

int RendCORE_TxtLyr_Sort(CJQ_Screen screen);

int RendCORE_Order_Cmp(const void* ptr_a, const void* ptr_b, const u16 ord_a, const u16 ord_b);

void RendCORE_LoadingScreen_Show(void);

void RendCORE_LoadingScreen_Hide(void);

#endif // RENDCORE_H