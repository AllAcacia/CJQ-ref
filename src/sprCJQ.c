/*
 * Filename: sprCJQ.c
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Handles sprite aggregation
 * into game scene objects.
 */

#include "sprCJQ.h"


void SprCJQ_RenderSprite_Init(CJQ_RenderSprite* rend_sprite, CJQ_Screen screen, CJQ_RendLayer lyr_i, int16_t order)
{
	rend_sprite->rend_layer_i = lyr_i;
	rend_sprite->screen = screen;
	rend_sprite->rend_order_in_layer = order;
	rend_sprite->is_visible = true;
}


void SprCJQ_LayerSprites_Init(CJQ_LayerSprites* rend_layer)
{
	rend_layer->arr_len = 0;
	rend_layer->is_dirty = false;

	for (uint16_t spr_ptr_i = 0; spr_ptr_i < REND_LYR_i_MAX_SPR; spr_ptr_i += 1) {
		(rend_layer->rend_sprite_ptrs)[spr_ptr_i] = NULL;
	}
}


void SprCJQ_RenderSprite_Show(CJQ_RenderSprite* rend_sprite)
{
	rend_sprite->is_visible = true;
}


void SprCJQ_RenderSprite_Hide(CJQ_RenderSprite* rend_sprite)
{
	rend_sprite->is_visible = false;
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