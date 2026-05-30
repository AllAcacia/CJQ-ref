/*
 * Filename: uiCJQ.c
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Contains GUI data structures
 * and functionality.
 */


#include "uiCJQ.h"


int UiCJQ_GridInit(CJQ_UiGrid* grid, size_t num_slots)
{
    if (num_slots < 1) {
        return EXIT_FAILURE;
    }

    grid->slots = calloc(num_slots, sizeof(CJQ_UiSlot));
    
    if (grid->slots == NULL) {
        printf("Could not allocate slots for UI grid... (no space on heap)\n");
        return EXIT_FAILURE;
    }

    grid->num_slots = num_slots;

    for (size_t i = 0; i < num_slots; i += 1) {
        UiCJQ_SlotInit(&grid->slots[i], i);
    }
    grid->slot_sel = &grid->slots[0];
    grid->slots[0].is_select = true;

    return EXIT_SUCCESS;
}


void UiCJQ_GridResetSelect(CJQ_UiGrid* grid)
{
    grid->slot_sel->is_select = false;
    grid->slots[0].is_select = true;
    grid->slot_sel = &grid->slots[0];
}


int UiCJQ_SlotInit(CJQ_UiSlot* slot, u8 sl_id)
{
    slot->is_select = false;
    slot->sl_id = sl_id;
    slot->slot_l = NULL;
    slot->slot_r = NULL;
    slot->slot_u = NULL;
    slot->slot_d = NULL;

    return EXIT_SUCCESS;
}


void UiCJQ_SlotSetAdj(CJQ_UiSlot* cntr, CJQ_UiSlot* adj, CJQ_Direction dir)
{
    switch (dir) {
        case DIR_LEFT:
            cntr->slot_l = (void*)adj;
            break;
        case DIR_RIGHT:
            cntr->slot_r = (void*)adj;
            break;
        case DIR_UP:
            cntr->slot_u = (void*)adj;
            break;
        case DIR_DOWN:
            cntr->slot_d = (void*)adj;
            break;
        default:
            break;
    }
}


bool UiCJQ_NavigateSceneSlots(CJQ_UiGrid* grid, CJQ_Direction dir)
{
  CJQ_UiSlot* new_slot_sel = NULL;

  switch (dir) {
    case DIR_LEFT:
        new_slot_sel = grid->slot_sel->slot_l;
        break;
    case DIR_RIGHT:
        new_slot_sel = grid->slot_sel->slot_r;
        break;
    case DIR_UP:
        new_slot_sel = grid->slot_sel->slot_u;
        break;
    case DIR_DOWN:
        new_slot_sel = grid->slot_sel->slot_d;
        break;
    default:
        break;
  }

  if (new_slot_sel != NULL) {
    grid->slot_sel->is_select = false;
    new_slot_sel->is_select = true;
    grid->slot_sel = new_slot_sel;
    return true;  // Indicate a new slot has been selected
  } else {
    return false; // Indicate no new slot was selected
  }
}