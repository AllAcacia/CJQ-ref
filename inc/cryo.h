/*
 * Filename: cryo.h
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Overarching module controlling
 * the Card-Jitsu Snow game!
 */

#ifndef CRYO_H
#define CRYO_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <3ds.h>
#include <citro2d.h>
#include "gamestate.h"
#include "cardCJQ.h"
#include "ctrli.h"
#include "sprCJQ.h"
#include "dynamicSS.h"
#include "rendCORE.h"

#include "sblol.h" // easter egg!

int CryoCJQ_Launch(void);

#endif // CRYO_H