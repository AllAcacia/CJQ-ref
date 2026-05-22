/*
 * Filename: rendCORE.c
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

#include "rendCORE.h"

static CJQ_LayerSprites SPR_LYR_COLLECTION[NUM_SCREENS][RendLayer_COUNT]; // array of all sprite layers for TOP & BOT screen

static CJQ_LoadMenuAssets load_menu_assets;

void RendCORE_Init(void)
{
    RendCORE_SprLyrCollection_Init(SPR_LYR_COLLECTION);
    
    SprCJQ_RenderSprite_Init(&load_menu_assets.menu_load_top_spr, TOP_SCREEN, RendLayer_BACKGND, INT16_MAX);
    SprCJQ_RenderSprite_Init(&load_menu_assets.menu_load_bot_spr, BOT_SCREEN, RendLayer_BACKGND, INT16_MAX);
    load_menu_assets.menu_load_sheet = C2D_SpriteSheetLoad(SPRITESHEET_PATH_LOADING_MENU);
    C2D_SpriteFromSheet(&load_menu_assets.menu_load_top_spr.c2d_spr, load_menu_assets.menu_load_sheet, 0);
    C2D_SpriteFromSheet(&load_menu_assets.menu_load_bot_spr.c2d_spr, load_menu_assets.menu_load_sheet, 1);
    RendCORE_SprLyr_AppendSprite(&load_menu_assets.menu_load_top_spr);
    RendCORE_SprLyr_AppendSprite(&load_menu_assets.menu_load_bot_spr);
    RendCORE_LoadingScreen_Hide();
}


void RendCORE_Exit(void)
{
    RendCORE_SprLyrCollection_Init(SPR_LYR_COLLECTION);
    C2D_SpriteSheetFree(load_menu_assets.menu_load_sheet);
    RendCORE_SprLyr_DeleteSprite(&load_menu_assets.menu_load_top_spr);
    RendCORE_SprLyr_DeleteSprite(&load_menu_assets.menu_load_bot_spr);
}


void RendCORE_Execute(void)
{
    // Clean messy layers
    for (size_t lyr_i = 0; lyr_i < RendLayer_COUNT; lyr_i += 1) {
        if (SPR_LYR_COLLECTION[TOP_SCREEN][lyr_i].is_dirty) {
            RendCORE_SprLyr_SortSprites(TOP_SCREEN, lyr_i); // clean TOP screen layers
        }
        if (SPR_LYR_COLLECTION[BOT_SCREEN][lyr_i].is_dirty) {
            RendCORE_SprLyr_SortSprites(BOT_SCREEN, lyr_i); // clean BOT screen layers
        }
    }

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    
    // TOP screen
    C2D_SceneBegin(GS_GetTopScreen());
    C2D_TargetClear(GS_GetTopScreen(), C2D_BLACK);
    RendCORE_Execute_Screen(SPR_LYR_COLLECTION[TOP_SCREEN]);

    // BOT screen
    C2D_SceneBegin(GS_GetBotScreen());
    C2D_TargetClear(GS_GetBotScreen(), C2D_BLACK);
    RendCORE_Execute_Screen(SPR_LYR_COLLECTION[BOT_SCREEN]);

    C3D_FrameEnd(0);
}


void RendCORE_Execute_Screen(CJQ_LayerSprites* lyr_collection)
{
    for (size_t lyr_i = 0; lyr_i < RendLayer_COUNT; lyr_i += 1) {
        CJQ_LayerSprites* lyr_i_obj = &(lyr_collection[lyr_i]);
        for (size_t spr_j = 0; spr_j < lyr_i_obj->arr_len; spr_j += 1) {
            CJQ_RenderSprite* spr_j_obj = (lyr_i_obj->rend_sprite_ptrs[spr_j]);
            if (spr_j_obj->is_visible) {
                C2D_DrawSprite(&(spr_j_obj->c2d_spr));
            }
        }
    }
}


void RendCORE_SprLyrCollection_Init(CJQ_LayerSprites collection[NUM_SCREENS][RendLayer_COUNT])
{
    for (uint16_t lyr_i = 0; lyr_i < RendLayer_COUNT; lyr_i += 1) {
        SprCJQ_LayerSprites_Init(&(collection[TOP_SCREEN][lyr_i]));
        SprCJQ_LayerSprites_Init(&(collection[BOT_SCREEN][lyr_i]));
    }
}


int RendCORE_SprLyr_AppendSprite(CJQ_RenderSprite* rend_sprite_ptr)
{
    // Get important data
    CJQ_Screen screen = rend_sprite_ptr->screen;
    CJQ_RendLayer lyr_i = rend_sprite_ptr->rend_layer_i;
    CJQ_LayerSprites* rend_lyr_i = &(SPR_LYR_COLLECTION[screen][lyr_i]);

    if (rend_lyr_i->arr_len < REND_LYR_i_MAX_SPR) {
        // Add pointer to array
        (rend_lyr_i->rend_sprite_ptrs)[rend_lyr_i->arr_len] = rend_sprite_ptr;

        // Update layer variables
        rend_lyr_i->arr_len += 1;
        rend_lyr_i->is_dirty = true;

        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}


int RendCORE_SprLyr_DeleteSprite(CJQ_RenderSprite* rend_sprite_ptr)
{
    // Find pointer
    CJQ_RendLayer lyr_i = rend_sprite_ptr->rend_layer_i;
    CJQ_Screen screen = rend_sprite_ptr->screen;
    CJQ_LayerSprites* rend_lyr_i = &(SPR_LYR_COLLECTION[screen][lyr_i]);
    CJQ_RenderSprite** rend_lyr_i_sprs = rend_lyr_i->rend_sprite_ptrs;
    
    int16_t index = RendCORE_FindSprPtr(rend_lyr_i_sprs, rend_sprite_ptr, rend_lyr_i->arr_len);

    if ((index != -1) && (rend_lyr_i->arr_len > 0)) { // if ptr exists
        rend_lyr_i_sprs[index] = rend_lyr_i_sprs[(rend_lyr_i->arr_len)-1];
        rend_lyr_i->arr_len -= 1;
        rend_lyr_i_sprs[rend_lyr_i->arr_len] = NULL;
        rend_lyr_i->is_dirty = true;
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}


int16_t RendCORE_FindSprPtr(CJQ_RenderSprite** arr, CJQ_RenderSprite* ptr, uint16_t arr_len)
{
    /* linear search algorithm from:
     * https://www.geeksforgeeks.org/dsa/linear-search/ */

    // Iterate over the array in order to
    // find the key x
    for (int i = 0; i < arr_len; i++)
        if (arr[i] == ptr)
            return i;
    return -1;
}


int RendCORE_SprLyr_SortSprites(CJQ_Screen screen, CJQ_RendLayer lyr_i)
{
    /* insertion sort algorithm from:
     * https://www.geeksforgeeks.org/dsa/insertion-sort-algorithm/ */
    
    CJQ_LayerSprites* rend_lyr_i = &(SPR_LYR_COLLECTION[screen][lyr_i]);
    rend_lyr_i->is_dirty = false;
    
    if (rend_lyr_i->arr_len > 1) {
        CJQ_RenderSprite** arr = rend_lyr_i->rend_sprite_ptrs;

        for (int i = 1; i < rend_lyr_i->arr_len; ++i) {
            CJQ_RenderSprite* key = arr[i];
            int j = i - 1;
            /* Move elements of arr[0..i-1], that are
            greater than key, to one position ahead
            of their current position */
            while ((j >= 0) && (RendCORE_spr_order_cmp(arr[j], key) > 0)) {
                arr[j + 1] = arr[j];
                j -= 1;
            }
            arr[j + 1] = key;
        }
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}


int RendCORE_spr_order_cmp(const CJQ_RenderSprite* spr_a_ptr, const CJQ_RenderSprite* spr_b_ptr)
{
  int16_t spr_a_order = spr_a_ptr->rend_order_in_layer;
  int16_t spr_b_order = spr_b_ptr->rend_order_in_layer;

  if (spr_a_order > spr_b_order) return 1;
  if (spr_a_order < spr_b_order) return -1;
  
  // Tiebreaker
  if (spr_a_ptr > spr_b_ptr) return 1;
  if (spr_a_ptr < spr_b_ptr) return -1;
  
  // Default case (same pointer)
  return 0;
}


void RendCORE_LoadingScreen_Show(void)
{
    load_menu_assets.menu_load_top_spr.is_visible = true;
    load_menu_assets.menu_load_bot_spr.is_visible = true;
}


void RendCORE_LoadingScreen_Hide(void)
{
    load_menu_assets.menu_load_top_spr.is_visible = false;
    load_menu_assets.menu_load_bot_spr.is_visible = false;
}