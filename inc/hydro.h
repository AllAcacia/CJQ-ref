/*
 * Filename: hydro.h
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Overarching module controlling
 * the Card-Jitsu Water game!
 */

#ifndef HYDRO_H
#define HYDRO_H

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

int HydroCJQ_Launch(void);

#endif // HYDRO_H