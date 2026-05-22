/*
 * Filename: ctrli.c
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Handles gathering control inputs.
 * (ignore previous commits where I
 * used an extern keyword, how blasphemous
 * in this day and age of setter and
 * getter functions ._. )
 */


#include "ctrli.h"


static CTRLi_State i_state;


void CTRLi_CaptureInputState(void)
{
    hidScanInput(); // scan first

	i_state.kUp = hidKeysUp();
	i_state.kDown = hidKeysDown();
	i_state.kHeld = hidKeysHeld();
	hidTouchRead(&(i_state.vtpad));
	hidCircleRead(&(i_state.vcpad));
	hidAccelRead(&(i_state.vaccl));
	hidGyroRead(&(i_state.vgyro));
}


CTRLi_State CTRLi_GetInputState(void)
{
	return i_state;
}