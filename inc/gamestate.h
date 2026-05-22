/*
 * Filename: gamestate.h
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Contains important definitions.
 */

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <3ds.h>
#include <citro2d.h>
#include <limits.h>

#define REFRESH_RATE_HZ 60
#define SIMUL_RATE_HZ 60
#define SBLOL_RATE_HZ 30
#define GAME_ANIM_HZ 12 // Have to check
#define MENU_SELECT KEY_SELECT
#define PROTO_SELECT KEY_A
#define PYRO_SELECT KEY_B
#define HYDRO_SELECT KEY_Y
#define CRYO_SELECT KEY_X
#define DEBUG_SELECT KEY_ZR
#define TOP_SCREEN_WIDTH 400
#define TOP_SCREEN_HEIGHT 240
#define BOTTOM_SCREEN_WIDTH 320
#define BOTTOM_SCREEN_HEIGHT 240
#define MAX_SIMUL_STEPS 3 // prevents overloading the simulation loop


typedef enum {
	MODE_MENU=0, // Menu
	MODE_PROTO,  // CJ Base
	MODE_PYRO,   // CJ Fire
	MODE_HYDRO,  // CJ Water
	MODE_CRYO,   // CJ Snow
    MODE_EXTRA   // Easter Egg
} CJQ_GameState;


void GS_Init(void);

void GS_DelayTicks(size_t tick_delay);

void GS_IncrTimeSec(void);

u64 GS_GetTickDelay_ms(u64 delay_ms);

u64 GS_GetTickDelay_hz(u64 delay_hz);

bool GS_CheckDelayTimer(u64 tick_ref, u64 delay_ticks);

void GS_SetGameState(CJQ_GameState new_gamestate);

CJQ_GameState GS_GetGameState(void);

bool GS_GetIsDebugMode(void);

C3D_RenderTarget* GS_GetTopScreen(void);

C3D_RenderTarget* GS_GetBotScreen(void);

void GS_ResetFrameTickNet(void);

void GS_IncrFrameTickNet(u64 tick_delta);

void GS_DecrFrameTickNet(u64 tick_delta);

u64 GS_GetFrameTickNet(void);

u64 GS_GetTickFrame(void);

void GS_SetTimeSec(u64 time);

u64 GS_GetTimeSec(void);

void GS_SetFrameTickPrv(u64 tick);

u64 GS_GetFrameTickPrv(void);

void GS_UpdTickNow(void);

u64 GS_GetTickNow(void);

u64 GS_GetSimulTicks(void);

u64 GS_GetRefreshTicks(void);

u64 GS_GetSBLOLAnimTicks(void);

u64 GS_GetGameAnimTicks(void);

#endif // GAMESTATE_H