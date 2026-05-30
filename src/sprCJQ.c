/*
 * Filename: sprCJQ.c
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Handles sprite aggregation
 * into game scene objects.
 */

#include "sprCJQ.h"


static C2D_TextBuf TXT_DYNAMIC_BUF; // Dynamic text buffer
static C2D_TextBuf TXT_STATIC_BUF;  // Static text buffer


void SprCJQ_RendSprite_Init(CJQ_RendSprite* rend_sprite, CJQ_Screen screen, CJQ_RendLayer lyr_i, int16_t order)
{
	rend_sprite->rend_layer_i = lyr_i;
	rend_sprite->screen = screen;
	rend_sprite->rend_order_in_layer = order;
	rend_sprite->is_visible = true;
}


int SprCJQ_RendText_Init(CJQ_RendText* rend_text, CJQ_Screen screen, char* msg, CJQ_TextBuffer buf, int flags, C2D_Font font, float x, float y, float scale, u32 colour)
{
	C2D_TextBuf* buf_obj;
	switch (buf) {
		case TxtBuf_DYNAMIC:
			buf_obj = &TXT_DYNAMIC_BUF;
			break;
		case TxtBuf_STATIC:
			buf_obj = &TXT_STATIC_BUF;
			break;
		default:
			return EXIT_FAILURE;
	}

	rend_text->rend_layer_i = RendLayer_UITXT;
	rend_text->screen = screen;
	rend_text->is_visible = true;
	rend_text->flags = flags;
	rend_text->buf = buf;

	rend_text->colour = colour;

	// Cartesian coordinates
	rend_text->x = x;
	rend_text->y = y;
	rend_text->z = 0.0f; // unspecified depth

	// Cartesian scaling (init uniformly)
	rend_text->scaleX = scale;
	rend_text->scaleY = scale;

	// Parse text
	if (font == NULL) {
		C2D_TextParse(&rend_text->c2d_txt, *buf_obj, msg);
	} else {
		C2D_TextFontParse(&rend_text->c2d_txt, font, *buf_obj, msg);
	}
	C2D_TextOptimize(&rend_text->c2d_txt);
	
	return EXIT_SUCCESS;
}


void SprCJQ_SpriteLayer_Init(CJQ_SpriteLayer* rend_layer)
{
	rend_layer->arr_len = 0;
	rend_layer->is_dirty = false;

	for (uint16_t spr_ptr_i = 0; spr_ptr_i < REND_LYR_i_MAX_SPR; spr_ptr_i += 1) {
		(rend_layer->rend_sprite_ptrs)[spr_ptr_i] = NULL;
	}
}


void SprCJQ_TextLayer_Init(CJQ_TextLayer* rend_layer)
{
	rend_layer->arr_len = 0;

	for (uint16_t txt_ptr_i = 0; txt_ptr_i < REND_LYR_i_MAX_SPR; txt_ptr_i += 1) {
		(rend_layer->rend_text_ptrs)[txt_ptr_i] = NULL;
	}
}


C2D_TextBuf* SprCJQ_GetDynamicTextBuf(void)
{
    return &TXT_DYNAMIC_BUF;
}


void SprCJQ_SetDynamicTextBuf(C2D_TextBuf buf)
{
	TXT_DYNAMIC_BUF = buf;
}


void SprCJQ_ClearDynamicTextBuf(void)
{
	C2D_TextBufClear(TXT_DYNAMIC_BUF);
}


void SprCJQ_DeleteDynamicTextBuf(void)
{
	C2D_TextBufDelete(TXT_DYNAMIC_BUF);
}


C2D_TextBuf* SprCJQ_GetStaticTextBuf(void)
{
    return &TXT_STATIC_BUF;
}


void SprCJQ_SetStaticTextBuf(C2D_TextBuf buf)
{
	TXT_STATIC_BUF = buf;
}


void SprCJQ_ClearStaticTextBuf(void)
{
	C2D_TextBufClear(TXT_STATIC_BUF);
}


void SprCJQ_DeleteStaticTextBuf(void)
{
	C2D_TextBufDelete(TXT_STATIC_BUF);
}


void SprCJQ_RendSprite_Show(CJQ_RendSprite* rend_sprite)
{
	rend_sprite->is_visible = true;
}


void SprCJQ_RendText_Show(CJQ_RendText* rend_text)
{
	rend_text->is_visible = true;
}


void SprCJQ_RendSprite_Hide(CJQ_RendSprite* rend_sprite)
{
	rend_sprite->is_visible = false;
}


void SprCJQ_RendText_Hide(CJQ_RendText* rend_text)
{
	rend_text->is_visible = false;
}


// example of bad practice, preferably use "C2D_SpriteFromSheet(C2D_Sprite sprite, C2D_Spritesheet sheet, size_t index)"
void loadSpritesFromSpritesheet(C2D_Sprite* ptr, C2D_SpriteSheet sheet, size_t numImages, float piv_x, float piv_y, uint16_t loc_x, uint16_t loc_y, float rot_d)
{
    if (!sheet) svcBreak(USERBREAK_PANIC);

    for (size_t i = 0; i < numImages; i += 1)
	{
		C2D_Sprite* s = &ptr[i];
		C2D_SpriteFromSheet(s, sheet, i);
		C2D_SpriteSetCenter(s, piv_x, piv_y);
		C2D_SpriteSetPos(s, loc_x, loc_y);
		C2D_SpriteRotateDegrees(s, rot_d);
	}
}