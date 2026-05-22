/*
 * Filename: pyro.h
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Overarching module controlling
 * the Card-Jitsu Fire game!
 */

#ifndef PYRO_H
#define PYRO_H

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

#include "sblol.h"

int PyroCJQ_Launch(void);

#endif // PYRO_H