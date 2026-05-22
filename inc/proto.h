/*
 * Filename: proto.c
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Overarching module controlling
 * the original Card-Jitsu game!
 */


#ifndef PROTO_H
#define PROTO_H

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

int ProtoCJQ_Launch(void);

#endif // PROTO_H