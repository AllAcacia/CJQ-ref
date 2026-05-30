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

static CJQ_SpriteLayer SPR_LYR_COLLECTION[NUM_SCREENS][RendLayer_COUNT]; // array of all sprite layers for TOP & BOT screen
static CJQ_TextLayer TXT_LYR_COLLECTION[NUM_SCREENS][TxtBuf_COUNT];      // array of text layer (singular) for TOP & BOT screen

static CJQ_LoadMenuAssets load_menu_assets;

void RendCORE_Init(void)
{
    // Init Layers
    RendCORE_SprLyrCollection_Init(SPR_LYR_COLLECTION);
    RendCORE_TxtLyrCollection_Init(TXT_LYR_COLLECTION);

    // Init Text Buffers
    SprCJQ_SetDynamicTextBuf(C2D_TextBufNew(BUF_GLYPHS_MAX));
    SprCJQ_SetStaticTextBuf(C2D_TextBufNew(BUF_GLYPHS_MAX));
    
    // Init Loading Menu
    load_menu_assets.menu_load_sheet = C2D_SpriteSheetLoad(SPRITESHEET_PATH_LOADING_MENU);
    load_menu_assets.menu_load_top_spr.screen = TOP_SCREEN;
    load_menu_assets.menu_load_bot_spr.screen = BOT_SCREEN;
    C2D_SpriteFromSheet(&load_menu_assets.menu_load_top_spr.c2d_spr, load_menu_assets.menu_load_sheet, 0);
    C2D_SpriteFromSheet(&load_menu_assets.menu_load_bot_spr.c2d_spr, load_menu_assets.menu_load_sheet, 1);
    RendCORE_SprLyr_AppendSprite(&load_menu_assets.menu_load_top_spr);
    RendCORE_SprLyr_AppendSprite(&load_menu_assets.menu_load_bot_spr);
    RendCORE_LoadingScreen_Hide();

    printf("Initialised RendCORE\n");
}


void RendCORE_Exit(void)
{
    SprCJQ_DeleteDynamicTextBuf();
    SprCJQ_DeleteStaticTextBuf();
    RendCORE_SprLyrCollection_Init(SPR_LYR_COLLECTION);
    C2D_SpriteSheetFree(load_menu_assets.menu_load_sheet);
    RendCORE_SprLyr_DeleteSprite(&load_menu_assets.menu_load_top_spr);
    RendCORE_SprLyr_DeleteSprite(&load_menu_assets.menu_load_bot_spr);
    printf("Exited RendCORE\n");
}


void RendCORE_Execute(void)
{
    // Clean messy layers
    for (size_t lyr_i = 0; lyr_i < RendLayer_COUNT; lyr_i += 1) {
        if (SPR_LYR_COLLECTION[TOP_SCREEN][lyr_i].is_dirty) {
            RendCORE_SprLyr_Sort(TOP_SCREEN, lyr_i); // clean TOP screen layers
        }
        if (SPR_LYR_COLLECTION[BOT_SCREEN][lyr_i].is_dirty) {
            RendCORE_SprLyr_Sort(BOT_SCREEN, lyr_i); // clean BOT screen layers
        }
    }

    // Clear and re-fill dynamic text buffer
    SprCJQ_ClearDynamicTextBuf();
    /*
     * Do stuff...
    */

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    
    // TOP screen
    C2D_SceneBegin(GS_GetTopScreen());
    C2D_TargetClear(GS_GetTopScreen(), C2D_BLACK);
    RendCORE_Execute_Screen_Spr(SPR_LYR_COLLECTION[TOP_SCREEN]);
    RendCORE_Execute_Screen_Txt(TXT_LYR_COLLECTION[TOP_SCREEN]);

    // BOT screen
    C2D_SceneBegin(GS_GetBotScreen());
    C2D_TargetClear(GS_GetBotScreen(), C2D_BLACK);
    RendCORE_Execute_Screen_Spr(SPR_LYR_COLLECTION[BOT_SCREEN]);
    RendCORE_Execute_Screen_Txt(TXT_LYR_COLLECTION[BOT_SCREEN]);

    C3D_FrameEnd(0);
}


void RendCORE_Execute_Screen_Spr(CJQ_SpriteLayer* lyr_collection)
{
    for (size_t lyr_i = 0; lyr_i < RendLayer_COUNT; lyr_i += 1) {
        CJQ_SpriteLayer* lyr_i_obj = &(lyr_collection[lyr_i]);
        for (size_t spr_j = 0; spr_j < lyr_i_obj->arr_len; spr_j += 1) {
            CJQ_RendSprite* spr_j_obj = (lyr_i_obj->rend_sprite_ptrs[spr_j]);
            if (spr_j_obj->is_visible) {
                C2D_DrawSprite(&(spr_j_obj->c2d_spr));
            }
        }
    }
}


void RendCORE_Execute_Screen_Txt(CJQ_TextLayer* lyr_obj)
{
    for (size_t buf_j=0; buf_j < TxtBuf_COUNT; buf_j += 1) {
        for (size_t txt_k = 0; txt_k < lyr_obj->arr_len; txt_k += 1) {
            CJQ_RendText* txt_k_obj = (lyr_obj->rend_text_ptrs[txt_k]);
            if (txt_k_obj->is_visible) {
                C2D_DrawText(&(txt_k_obj->c2d_txt),
                               txt_k_obj->flags,
                               txt_k_obj->x,
                               txt_k_obj->y,
                               txt_k_obj->z,
                               txt_k_obj->scaleX,
                               txt_k_obj->scaleY,
                               txt_k_obj->colour);
            }
        }   
    }
}


void RendCORE_SprLyrCollection_Init(CJQ_SpriteLayer collection[NUM_SCREENS][RendLayer_COUNT])
{
    for (uint16_t lyr_i = 0; lyr_i < RendLayer_COUNT; lyr_i += 1) {
        SprCJQ_SpriteLayer_Init(&(collection[TOP_SCREEN][lyr_i]));
        SprCJQ_SpriteLayer_Init(&(collection[BOT_SCREEN][lyr_i]));
    }
    printf("Initialised sprite layer collection\n");
}


void RendCORE_TxtLyrCollection_Init(CJQ_TextLayer collection[NUM_SCREENS][TxtBuf_COUNT])
{
    for (size_t buf_i=0; buf_i < TxtBuf_COUNT; buf_i += 1) {
        SprCJQ_TextLayer_Init(&(collection[TOP_SCREEN][buf_i]));
        SprCJQ_TextLayer_Init(&(collection[BOT_SCREEN][buf_i]));
    }
    printf("Initialised text layer collection\n");
}


int RendCORE_SprLyr_AppendSprite(CJQ_RendSprite* rend_sprite_ptr)
{
    // Get important data
    CJQ_Screen screen = rend_sprite_ptr->screen;
    CJQ_RendLayer lyr_i = rend_sprite_ptr->rend_layer_i;
    CJQ_SpriteLayer* rend_lyr_i = &(SPR_LYR_COLLECTION[screen][lyr_i]);

    if (rend_lyr_i->arr_len < REND_LYR_i_MAX_SPR) {
        // Add pointer to array
        (rend_lyr_i->rend_sprite_ptrs)[rend_lyr_i->arr_len] = rend_sprite_ptr;

        // Update layer variables
        rend_lyr_i->arr_len += 1;
        rend_lyr_i->is_dirty = true;
        printf("Successfully appended sprite to collection\n");
        return EXIT_SUCCESS;
    } else {
        printf("Could not append sprite to collection... (full)\n");
        return EXIT_FAILURE;
    }
}


int RendCORE_TxtLyr_AppendText(CJQ_RendText* rend_text_ptr)
{
    // Get important data
    CJQ_Screen screen = rend_text_ptr->screen;
    CJQ_TextBuffer buf = rend_text_ptr->buf;
    CJQ_TextLayer* rend_lyr_i = &(TXT_LYR_COLLECTION[screen][buf]);

    if (rend_lyr_i->arr_len < REND_LYR_i_MAX_SPR) {
        // Add pointer to array
        (rend_lyr_i->rend_text_ptrs)[rend_lyr_i->arr_len] = rend_text_ptr;

        // Update layer variables
        rend_lyr_i->arr_len += 1;
        printf("Successfully appended text to collection\n");
        return EXIT_SUCCESS;
    } else {
        printf("Could not append text to collection... (full)\n");
        return EXIT_FAILURE;
    }
}


int RendCORE_SprLyr_DeleteSprite(CJQ_RendSprite* rend_sprite_ptr)
{
    // Find pointer
    CJQ_RendLayer lyr_i = rend_sprite_ptr->rend_layer_i;
    CJQ_Screen screen = rend_sprite_ptr->screen;
    CJQ_SpriteLayer* rend_lyr_i = &(SPR_LYR_COLLECTION[screen][lyr_i]);
    CJQ_RendSprite** rend_lyr_i_sprs = rend_lyr_i->rend_sprite_ptrs;
    
    int16_t index = RendCORE_FindSprPtr(rend_lyr_i_sprs, rend_sprite_ptr, rend_lyr_i->arr_len);

    if ((index != -1) && (rend_lyr_i->arr_len > 0)) { // if ptr exists
        rend_lyr_i_sprs[index] = rend_lyr_i_sprs[(rend_lyr_i->arr_len)-1];
        rend_lyr_i->arr_len -= 1;
        rend_lyr_i_sprs[rend_lyr_i->arr_len] = NULL;
        rend_lyr_i->is_dirty = true;
        printf("Successfully deleted sprite from collection\n");
        return EXIT_SUCCESS;
    } else {
        printf("Could not delete sprite from collection... (does not exist)\n");
        return EXIT_FAILURE;
    }
}


int RendCORE_TxtLyr_DeleteText(CJQ_RendText* rend_text_ptr)
{
    // Find pointer
    CJQ_Screen screen = rend_text_ptr->screen;
    CJQ_TextBuffer buf = rend_text_ptr->buf;
    CJQ_TextLayer* rend_lyr_i = &(TXT_LYR_COLLECTION[screen][buf]);
    CJQ_RendText** rend_lyr_i_txts = rend_lyr_i->rend_text_ptrs;
    
    int16_t index = RendCORE_FindTxtPtr(rend_lyr_i_txts, rend_text_ptr, rend_lyr_i->arr_len);

    if ((index != -1) && (rend_lyr_i->arr_len > 0)) { // if ptr exists
        rend_lyr_i_txts[index] = rend_lyr_i_txts[(rend_lyr_i->arr_len)-1];
        rend_lyr_i->arr_len -= 1;
        rend_lyr_i_txts[rend_lyr_i->arr_len] = NULL;
        printf("Successfully deleted text from collection\n");
        return EXIT_SUCCESS;
    } else {
        printf("Could not delete text from collection... (does not exist)\n");
        return EXIT_FAILURE;
    }
}


int16_t RendCORE_FindSprPtr(CJQ_RendSprite** arr, CJQ_RendSprite* ptr, uint16_t arr_len)
{
    /* linear search algorithm from:
     * https://www.geeksforgeeks.org/dsa/linear-search/ */

    // Iterate over the array in order to
    // find the key x
    for (int i = 0; i < arr_len; i++)
        if (arr[i] == ptr) {
            printf("Found sprite pointer\n");
            return i;
        }
    return -1;
}


int16_t RendCORE_FindTxtPtr(CJQ_RendText** arr, CJQ_RendText* ptr, uint16_t arr_len)
{
    /* linear search algorithm from:
     * https://www.geeksforgeeks.org/dsa/linear-search/ */

    // Iterate over the array in order to
    // find the key x
    for (int i = 0; i < arr_len; i++)
        if (arr[i] == ptr) {
            printf("Found text pointer\n");
            return i;
        }
    return -1;
}


int RendCORE_SprLyr_Sort(CJQ_Screen screen, CJQ_RendLayer lyr_i)
{
    /* insertion sort algorithm from:
     * https://www.geeksforgeeks.org/dsa/insertion-sort-algorithm/ */
    
    CJQ_SpriteLayer* rend_lyr_i = &(SPR_LYR_COLLECTION[screen][lyr_i]);
    rend_lyr_i->is_dirty = false;
    
    if (rend_lyr_i->arr_len > 1) {
        CJQ_RendSprite** arr = rend_lyr_i->rend_sprite_ptrs;

        for (int i = 1; i < rend_lyr_i->arr_len; ++i) {
            CJQ_RendSprite* key = arr[i];
            int j = i - 1;
            /* Move elements of arr[0..i-1], that are
            greater than key, to one position ahead
            of their current position */
            while ((j >= 0) && (RendCORE_Order_Cmp((void*)arr[j], (void*)key, arr[j]->rend_order_in_layer, key->rend_order_in_layer) > 0)) {
                arr[j + 1] = arr[j];
                j -= 1;
            }
            arr[j + 1] = key;
        }
        printf("Sorted sprite layer %d\n", lyr_i);
        return EXIT_SUCCESS;
    } else {
        printf("Could not sort sprite layer %d... (<=1 items)\n", lyr_i);
        return EXIT_FAILURE;
    }
}


int RendCORE_Order_Cmp(const void* ptr_a, const void* ptr_b, const u16 ord_a, const u16 ord_b)
{
  if (ord_a > ord_b) return 1;
  if (ord_a < ord_b) return -1;
  
  // Tiebreaker
  if (ptr_a > ptr_b) return 1;
  if (ptr_a < ptr_b) return -1;
  
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