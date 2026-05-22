/*
 * Filename: cardsCJQ.c
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Card metadata.
 */

#include "cardCJQ.h"


static CJQ_Card* CARD_COLLECTION = NULL;
static size_t CARD_COLLECTION_CARD_CNT = 0;
static size_t CARD_COLLECTION_BYTE_CNT = 0;

int Cards_LoadDatabase(void)
{
    printf("Attempting to load cards into database...\n");

    if (CARD_COLLECTION != NULL) {
        return EXIT_FAILURE; // if database already is loaded
    }

    size_t bytes_used = 0;

    // Open file
    printf("Attempting to open %s\n", CARDS_DATABASE_PATH);
    FILE* db = fopen(CARDS_DATABASE_PATH, "rb");
    if (db == NULL) {
        printf("%s cannot be opened or does not exist\n", CARDS_DATABASE_PATH);
        return EXIT_FAILURE;
    }
    printf("Opened database file\n");

    // Read number of entries
    u16 num_entries;

    if (fread(&num_entries, sizeof(u16), 1, db) != 1) {
        fclose(db);
        return EXIT_FAILURE;
    }

    num_entries = __builtin_bswap16(num_entries);
    
    // Allocate memory for all these cards
    CARD_COLLECTION_CARD_CNT = (size_t)num_entries;
    printf("Database contains %zu elements\n", CARD_COLLECTION_CARD_CNT);
    CARD_COLLECTION = calloc(CARD_COLLECTION_CARD_CNT, sizeof(CJQ_Card));
    if (CARD_COLLECTION != NULL) {
        printf("Allocated %zu bytes for the database\n", CARD_COLLECTION_CARD_CNT*sizeof(CJQ_Card));
    } else {
        printf("Failed to allocate %zu bytes for the database\n", CARD_COLLECTION_CARD_CNT*sizeof(CJQ_Card));
        fclose(db);
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < CARD_COLLECTION_CARD_CNT; i += 1) {
        u32 card_data;
        if (fread(&card_data, sizeof(u32), 1, db) != 1) {
            fclose(db);
            printf("Failed to read 4-byte entry header\n");
            return EXIT_FAILURE;
        }
        card_data = __builtin_bswap32(card_data);
        /*
         * Maybe check parities???
         */
        u16 id = (card_data & DB_HDR_ID_MASK) >> DB_HDR_ID_SHIFT;
        u8 rank = (card_data & DB_HDR_RANK_MASK) >> DB_HDR_RANK_SHIFT;
        bool power = (card_data & DB_HDR_ISPOWER_MASK) >> DB_HDR_ISPOWER_SHIFT;
        CJQ_CardElement element = (card_data & DB_HDR_ELEMENT_MASK) >> DB_HDR_ELEMENT_SHIFT;
        CJQ_CardEffect effect = (card_data & DB_HDR_POWEREFF_MASK) >> DB_HDR_POWEREFF_SHIFT;
        CJQ_CardColour colour = (card_data & DB_HDR_COLOUR_MASK) >> DB_HDR_COLOUR_SHIFT;
        
        u8 namesize = (card_data & DB_HDR_NAMESIZE_MASK) >> DB_HDR_NAMESIZE_SHIFT;
        char* name = calloc(namesize+1, sizeof(char));
        if (name == NULL) {
            printf("Could not allocate memory for card-name\n");
            return EXIT_FAILURE;
        }

        for (size_t j = 0; j < namesize; j += 1) {
            u8 name_c;
            if (fread(&name_c, sizeof(u8), 1, db) == 1) {
                name[j] = (char)name_c;
            } else {
                fclose(db);
                printf("Failed to read card-name characters\n");
                return EXIT_FAILURE;
            }
        } name[namesize] = '\0';
        
        Cards_CardInit(&CARD_COLLECTION[i], name, id, rank, power, element, effect, colour);
        Cards_CardDataPrint(&CARD_COLLECTION[i]);

        bytes_used += sizeof(CJQ_Card) + namesize + 1;
    }

    CARD_COLLECTION_BYTE_CNT = bytes_used;
    printf("Loaded %zu cards into memory (%zu B)\n", CARD_COLLECTION_CARD_CNT, CARD_COLLECTION_BYTE_CNT);
    printf("Closing database file...\n");
    fclose(db);

    return EXIT_SUCCESS;
}


void Cards_CloseDatabase(void)
{
    printf("Attempting to clear cards from database...\n");

    for (size_t i = 0; i < CARD_COLLECTION_CARD_CNT; i += 1) {
        free(CARD_COLLECTION[i].name);
    }

    if (CARD_COLLECTION != NULL) {
        free(CARD_COLLECTION);
        CARD_COLLECTION = NULL;
        printf("Cleared %zu cards from memory (%zu B)\n", CARD_COLLECTION_CARD_CNT, CARD_COLLECTION_BYTE_CNT);
        CARD_COLLECTION_CARD_CNT = 0;
    }
}


void Cards_CardInit(CJQ_Card* card, char* name, u16 id, u8 rank, bool power, CJQ_CardElement element, CJQ_CardEffect effect, CJQ_CardColour colour)
{
    card->name = name;
    card->id = id;
    card->rank = rank;
    card->power = power;
    card->element = element;
    card->effect = effect;
    card->colour = colour;
}


CJQ_Card Cards_GetCardFromDB(size_t index)
{
    return CARD_COLLECTION[index];
}


size_t Cards_GetCollectionCnt(void)
{
    return CARD_COLLECTION_CARD_CNT;
}


void Cards_CardDataPrint(CJQ_Card* card)
{
    printf("%5hu", card->id);
    printf(" | ");
    printf("%2u", card->rank);
    printf(" | ");
    switch (card->element) {
        case CARD_FIRE:
            printf("fire ");
            break;
        case CARD_WATER:
            printf("water");
            break;
        case CARD_SNOW:
            printf("snow ");
            break;
    }
    printf(" | ");
    switch (card->colour) {
        case CARD_RED:
            printf("red   ");
            break;
        case CARD_ORANGE:
            printf("orange");
            break;
        case CARD_YELLOW:
            printf("yellow");
            break;
        case CARD_GREEN:
            printf("green ");
            break;
        case CARD_BLUE:
            printf("blue  ");
            break;
        case CARD_PURPLE:
            printf("purple");
            break;
    }
    printf(" | ");
    switch (card->power) {
        case true:
            printf("pwr");
            break;
        case false:
            printf("non");
            break;
    }
    printf(" | ");
    switch (card->effect)
    {
        case CARD_NOPOWER:                printf("na ");  break;

        case CARD_DISCARD_FIRE:          printf("df ");  break;
        case CARD_DISCARD_WATER:         printf("dw ");  break;
        case CARD_DISCARD_SNOW:          printf("ds ");  break;

        case CARD_DISCARD_RED_MONO:      printf("dr ");  break;
        case CARD_DISCARD_ORANGE_MONO:   printf("do ");  break;
        case CARD_DISCARD_YELLOW_MONO:   printf("dy ");  break;
        case CARD_DISCARD_GREEN_MONO:    printf("dg ");  break;
        case CARD_DISCARD_BLUE_MONO:     printf("db ");  break;
        case CARD_DISCARD_PURPLE_MONO:   printf("dp ");  break;

        case CARD_DISCARD_RED_MULT:      printf("dmr"); break;
        case CARD_DISCARD_ORANGE_MULT:   printf("dmo"); break;
        case CARD_DISCARD_YELLOW_MULT:   printf("dmy"); break;
        case CARD_DISCARD_GREEN_MULT:    printf("dmg"); break;
        case CARD_DISCARD_BLUE_MULT:     printf("dmb"); break;
        case CARD_DISCARD_PURPLE_MULT:   printf("dmp"); break;

        case CARD_FIRE_TO_SNOW:          printf("fs ");  break;
        case CARD_SNOW_TO_WATER:         printf("sw ");  break;
        case CARD_WATER_TO_FIRE:         printf("wf ");  break;

        case CARD_BLOCK_FIRE:            printf("bf ");  break;
        case CARD_BLOCK_WATER:           printf("bw ");  break;
        case CARD_BLOCK_SNOW:            printf("bs ");  break;

        case CARD_POWER_REVERSAL:        printf("pr ");  break;
        case CARD_PLUS_TWO_USR:          printf("pt ");  break;
        case CARD_PLUS_TWO_OPP:          printf("mt ");  break;
    }
    printf(" | ");
    if (card->name == NULL) {
        printf("<no-name>");
    } else {
        printf("%s", card->name);
    }
    printf("\n");
}


int8_t Cards_ScrollCards(uint8_t* element, uint8_t* rank)
{
    int8_t has_changed = false;

	CTRLi_State i_state = CTRLi_GetInputState();

    if (i_state.kDown & KEY_DUP) {
        if (*rank < CARD_RANK_MAX-1) {
            *rank += 1;
        } else {
            *rank = CARD_RANK_MIN;
        }
        has_changed = true;
    } else if (i_state.kDown & KEY_DDOWN) {
        if (*rank > CARD_RANK_MIN) {
            *rank -= 1;
        } else {
            *rank = CARD_RANK_MAX-1;
        }
        has_changed = true;
    }
    if (i_state.kDown & KEY_DRIGHT) {
        if (*element < CARD_SNOW) {
            *element += 1;
        } else {
            *element = CARD_FIRE;
        }
        has_changed = true;
    } else if (i_state.kDown & KEY_DLEFT) {
        if (*element > CARD_FIRE) {
            *element -= 1;
        } else {
            *element = CARD_SNOW;
        }
        has_changed = true;
    }

    return has_changed;
}