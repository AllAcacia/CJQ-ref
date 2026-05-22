/*
 * Filename: cardsCJQ.h
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Card metadata.
 */

#ifndef CARDS_H
#define CARDS_H

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "ctrli.h"

/*-------------------------
  SPRITESHEET PATHS
-------------------------*/
#define SPRITESHEET_PATH_CARDS_FIRE "romfs:/gfx/cards_basic_f.t3x"
#define SPRITESHEET_PATH_CARDS_WATER "romfs:/gfx/cards_basic_w.t3x"
#define SPRITESHEET_PATH_CARDS_SNOW "romfs:/gfx/cards_basic_s.t3x"

/*-------------------------
  DATABASE DEFINITIONS
-------------------------*/
#define CARDS_DATABASE_PATH "romfs:/data/cards_db.bin"

#define DB_HDR_LEN 32 // bits
#define DB_HDR_ID_MASK_LEN 10
#define DB_HDR_RANK_MASK_LEN 4
#define DB_HDR_ELEMENT_MASK_LEN 2
#define DB_HDR_COLOUR_MASK_LEN 3
#define DB_HDR_ISPOWER_MASK_LEN 1
#define DB_HDR_POWEREFF_MASK_LEN 5
#define DB_HDR_NAMESIZE_MASK_LEN 6
#define DB_HDR_PARITY_MASK_LEN 1

#define DB_HDR_ID_SHIFT (DB_HDR_LEN-DB_HDR_ID_MASK_LEN)
#define DB_HDR_RANK_SHIFT (DB_HDR_ID_SHIFT-DB_HDR_RANK_MASK_LEN)
#define DB_HDR_ELEMENT_SHIFT (DB_HDR_RANK_SHIFT-DB_HDR_ELEMENT_MASK_LEN)
#define DB_HDR_COLOUR_SHIFT (DB_HDR_ELEMENT_SHIFT-DB_HDR_COLOUR_MASK_LEN)
#define DB_HDR_ISPOWER_SHIFT (DB_HDR_COLOUR_SHIFT-DB_HDR_ISPOWER_MASK_LEN)
#define DB_HDR_POWEREFF_SHIFT (DB_HDR_ISPOWER_SHIFT-DB_HDR_POWEREFF_MASK_LEN)
#define DB_HDR_NAMESIZE_SHIFT (DB_HDR_POWEREFF_SHIFT-DB_HDR_NAMESIZE_MASK_LEN)
#define DB_HDR_PARITY_SHIFT 0 // last bit of 32-bit header

#define DB_HDR_ID_MASK (0b1111111111 << DB_HDR_ID_SHIFT)
#define DB_HDR_RANK_MASK (0b1111 << DB_HDR_RANK_SHIFT)
#define DB_HDR_ELEMENT_MASK (0b11 << DB_HDR_ELEMENT_SHIFT)
#define DB_HDR_COLOUR_MASK (0b111 << DB_HDR_COLOUR_SHIFT)
#define DB_HDR_ISPOWER_MASK (0b1 << DB_HDR_ISPOWER_SHIFT)
#define DB_HDR_POWEREFF_MASK (0b11111 << DB_HDR_POWEREFF_SHIFT)
#define DB_HDR_NAMESIZE_MASK (0b111111 << DB_HDR_NAMESIZE_SHIFT)
#define DB_HDR_PARITY_MASK (0b1 << DB_HDR_PARITY_SHIFT)


typedef enum {
    CARD_RANK_MIN=1,
    CARD_ELEMENT_COUNT=3,
    CARD_RANK_MAX=12,
    CARD_SHEET_LEN=72 // Colour x Rank combination
} CJQ_CardData;

typedef enum {
    CARD_FIRE=0,
    CARD_WATER,
    CARD_SNOW,
} CJQ_CardElement;

typedef enum {
    CARD_RED=0,
    CARD_ORANGE,
    CARD_YELLOW,
    CARD_GREEN,
    CARD_BLUE,
    CARD_PURPLE
} CJQ_CardColour;

typedef enum {
    CARD_NOPOWER=0,
    CARD_DISCARD_FIRE,
    CARD_DISCARD_WATER,
    CARD_DISCARD_SNOW,
    CARD_DISCARD_RED_MONO,
    CARD_DISCARD_ORANGE_MONO,
    CARD_DISCARD_YELLOW_MONO,
    CARD_DISCARD_GREEN_MONO,
    CARD_DISCARD_BLUE_MONO,
    CARD_DISCARD_PURPLE_MONO,
    CARD_DISCARD_RED_MULT,
    CARD_DISCARD_ORANGE_MULT,
    CARD_DISCARD_YELLOW_MULT,
    CARD_DISCARD_GREEN_MULT,
    CARD_DISCARD_BLUE_MULT,
    CARD_DISCARD_PURPLE_MULT,
    CARD_FIRE_TO_SNOW,
    CARD_SNOW_TO_WATER,
    CARD_WATER_TO_FIRE,
    CARD_BLOCK_FIRE,
    CARD_BLOCK_WATER,
    CARD_BLOCK_SNOW,
    CARD_POWER_REVERSAL,
    CARD_PLUS_TWO_USR,
    CARD_PLUS_TWO_OPP
} CJQ_CardEffect;

typedef struct {
    char* name;              // Optional, otherwise NULL
    u16 id;                  // >=1
    u8 rank;                 // Typically 1-12
    bool power;              // Is card powered?
    CJQ_CardElement element; // Fire/Water/Snow
    CJQ_CardEffect effect;   // Side-effect of power card
    CJQ_CardColour colour;   // Card colour
} CJQ_Card;


// Open cards_db.bin and extract cards
int Cards_LoadDatabase(void);

// Clear all data in database from memory
void Cards_CloseDatabase(void);

// Initialise card object
void Cards_CardInit(CJQ_Card* card, char* name, u16 id, u8 rank, bool power, CJQ_CardElement element, CJQ_CardEffect effect, CJQ_CardColour colour);

// Extract card from loaded DB
CJQ_Card Cards_GetCardFromDB(size_t index);

// Return count of cards in collection
size_t Cards_GetCollectionCnt(void);

// Print card data
void Cards_CardDataPrint(CJQ_Card* card);

// Techdemo - scroll cards
int8_t Cards_ScrollCards(uint8_t* element, uint8_t* rank);

#endif // CARDS_H