/*
 * Filename: sprCJQ.h
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Handles sprite aggregation
 * into game scene objects.
 */

#ifndef SPRCJQ_H
#define SPRCJQ_H

#include <citro2d.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <3ds.h> // is this needed?

#define C2D_MAX_SPRITES 768

#define C2D_BLACK C2D_Color32(0, 0, 0, 255)
#define C2D_WHITE C2D_Color32(255, 255, 255, 255)

#define REND_LYR_i_MAX_SPR 64 // Layers can contain a maximum of this number


// | SPRITESHEET PATHS |
#define SPRITESHEET_PATH_MAIN_MENU "romfs:/gfx/menu_main.t3x"
#define SPRITESHEET_PATH_LOADING_MENU "romfs:/gfx/menu_load.t3x"
#define SPRITESHEET_PATH_UI_BUTTONS "romfs:/gfx/ui_buttons.t3x"


typedef enum {
	TOP_SCREEN=0,
	BOT_SCREEN,
    NUM_SCREENS
} CJQ_Screen;


typedef enum {
    RendLayer_BACKGND = 0,
    RendLayer_BACKFX,
    RendLayer_CHARA,
    RendLayer_FRNTFX,
    RendLayer_FRNTGND,
    RendLayer_UIFX,
    RendLayer_UITXT,
    RendLayer_COUNT
} CJQ_RendLayer;


typedef struct {
    CJQ_RendLayer rend_layer_i;
    C2D_Sprite c2d_spr;      // Reference to sprite object
    CJQ_Screen screen;
    int16_t rend_order_in_layer; // Usually left to 0
    bool is_visible;
} CJQ_RenderSprite;


typedef struct {
    CJQ_RenderSprite* rend_sprite_ptrs[REND_LYR_i_MAX_SPR]; // Reference to sprite object pointers
    uint16_t arr_len; // initially 0
    bool is_dirty;    // flag to signal whether a sort is needed;
} CJQ_LayerSprites;


void SprCJQ_RenderSprite_Init(CJQ_RenderSprite* rend_sprite, CJQ_Screen screen, CJQ_RendLayer lyr_i, int16_t order);

void SprCJQ_LayerSprites_Init(CJQ_LayerSprites* rend_layer);

void SprCJQ_RenderSprite_Show(CJQ_RenderSprite* rend_sprite);

void SprCJQ_RenderSprite_Hide(CJQ_RenderSprite* rend_sprite);

// example of bad practice, preferably use "C2D_SpriteFromSheet(C2D_Sprite sprite, C2D_Spritesheet sheet, size_t index)"
void loadSpritesFromSpritesheet(C2D_Sprite* ptr, C2D_SpriteSheet sheet, size_t numImages, float piv_x, float piv_y, uint16_t loc_x, uint16_t loc_y, float rot_d);

#endif // SPRCJQ_H
