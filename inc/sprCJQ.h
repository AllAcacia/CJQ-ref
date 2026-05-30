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
    RendLayer_BACKGND=0,
    RendLayer_BACKFX,
    RendLayer_CHARA,
    RendLayer_FRNTFX,
    RendLayer_FRNTGND,
    RendLayer_UIFX,
    RendLayer_COUNT,
    RendLayer_UITXT // Honorary "sprite" layer, not included in count as only one text layer should be needed
} CJQ_RendLayer;


typedef enum {
    TxtBuf_STATIC=0,
    TxtBuf_DYNAMIC,
    TxtBuf_COUNT
} CJQ_TextBuffer;


typedef struct {
    CJQ_RendLayer rend_layer_i;
    C2D_Sprite c2d_spr;      // Reference to sprite object
    CJQ_Screen screen;
    int16_t rend_order_in_layer; // Usually left to 0
    bool is_visible;
} CJQ_RendSprite;


typedef struct {
    CJQ_RendLayer rend_layer_i;
    C2D_Text c2d_txt;       // Reference to text object
    CJQ_Screen screen;
    CJQ_TextBuffer buf;
    bool is_visible;
    u32 flags;
    float x;
    float y;
    float z;
    float scaleX;
    float scaleY;
    u32 colour;
} CJQ_RendText;


typedef struct {
    CJQ_RendSprite* rend_sprite_ptrs[REND_LYR_i_MAX_SPR]; // Reference to sprite object pointers
    uint16_t arr_len; // initially 0
    bool is_dirty;    // flag to signal whether a sort is needed;
} CJQ_SpriteLayer;


typedef struct {
    CJQ_RendText* rend_text_ptrs[REND_LYR_i_MAX_SPR]; // Reference to sprite object pointers
    uint16_t arr_len; // initially 0
} CJQ_TextLayer;


void SprCJQ_RendSprite_Init(CJQ_RendSprite* rend_sprite, CJQ_Screen screen, CJQ_RendLayer lyr_i, int16_t order);

int SprCJQ_RendText_Init(CJQ_RendText* rend_text, CJQ_Screen screen, char* msg, CJQ_TextBuffer buf, int flags, C2D_Font font, float x, float y, float scale, u32 color);

void SprCJQ_SpriteLayer_Init(CJQ_SpriteLayer* rend_layer);

void SprCJQ_TextLayer_Init(CJQ_TextLayer* rend_layer);

C2D_TextBuf* SprCJQ_GetDynamicTextBuf(void);

void SprCJQ_SetDynamicTextBuf(C2D_TextBuf buf);

void SprCJQ_ClearDynamicTextBuf(void);

void SprCJQ_DeleteDynamicTextBuf(void);

C2D_TextBuf* SprCJQ_GetStaticTextBuf(void);

void SprCJQ_SetStaticTextBuf(C2D_TextBuf buf);

void SprCJQ_ClearStaticTextBuf(void);

void SprCJQ_DeleteStaticTextBuf(void);

void SprCJQ_RendSprite_Show(CJQ_RendSprite* rend_sprite);

void SprCJQ_RendText_Show(CJQ_RendText* rend_text);

void SprCJQ_RendSprite_Hide(CJQ_RendSprite* rend_sprite);

void SprCJQ_RendText_Hide(CJQ_RendText* rend_text);

// example of bad practice, preferably use "C2D_SpriteFromSheet(C2D_Sprite sprite, C2D_Spritesheet sheet, size_t index)"
void loadSpritesFromSpritesheet(C2D_Sprite* ptr, C2D_SpriteSheet sheet, size_t numImages, float piv_x, float piv_y, uint16_t loc_x, uint16_t loc_y, float rot_d);

#endif // SPRCJQ_H
