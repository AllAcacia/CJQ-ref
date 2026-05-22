/*
 * Filename: ctrli.h
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Handles gathering control inputs.
 * (ignore previous commits where I
 * used an extern keyword, how blasphemous
 * in this day and age of setter and
 * getter functions ._. )
 */

#ifndef CTRLI_H
#define CTRLI_H

#include <3ds.h>


typedef struct {
    u32 kUp;   // reads inputs released on last frame
    u32 kDown; // reads inputs pressed on this frame
    u32 kHeld; // reads inputs currently held
    circlePosition vcpad;     // Circle-Pad vector
    // circlePosition vcstick;   // C-Stick vector, unsure how to capture
    accelVector vaccl;        // Accelerometer vector
    angularRate vgyro;        // Gyroscope vector
    touchPosition vtpad;      // Touchpad vector
} CTRLi_State;


void CTRLi_CaptureInputState(void);

CTRLi_State CTRLi_GetInputState(void);

#endif // CTRLI_H