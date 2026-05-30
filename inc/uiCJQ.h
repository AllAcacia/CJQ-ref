/*
 * Filename: uiCJQ.h
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Contains GUI data structures
 * and functionality.
 */

#ifndef UICJQ_H
#define UICJQ_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "ctrli.h"

typedef enum {
    DIR_LEFT=0,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN,
    DIR_REST
} CJQ_Direction; // Maybe put this in a more generic header file to not reuse code

typedef struct {
    u16 sl_id;
    // Pointers to other slots, init. NULL
    void* slot_l;   // adj. slot to left
    void* slot_r;   // adj. slot to right
    void* slot_u;   // adj. slot upwards
    void* slot_d;   // adj. slot downwards
    bool is_select; // Is this slot selected?
} CJQ_UiSlot;

typedef struct {
    CJQ_UiSlot* slots;    // Pointer to slots, dynamically allocated
    CJQ_UiSlot* slot_sel; // Pointer to selected slot in array
    size_t num_slots;     // How many slots are kept in this scene
} CJQ_UiGrid;

int UiCJQ_GridInit(CJQ_UiGrid* grid, size_t num_slots);

int UiCJQ_SlotInit(CJQ_UiSlot* slot, u8 sl_id);

void UiCJQ_SlotSetAdj(CJQ_UiSlot* cntr, CJQ_UiSlot* adj, CJQ_Direction dir);

bool UiCJQ_NavigateSceneSlots(CJQ_UiGrid* grid, CJQ_Direction dir);

#endif // UICJQ_H