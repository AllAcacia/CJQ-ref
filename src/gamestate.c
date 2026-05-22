/*
 * Filename: gamestate.c
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Contains important definitions.
 */


#include "gamestate.h"

static u64 tick_now = 0;

static u64 frame_tick_prv = 0;
static u64 frame_tick_net = 0;

static u64 timer_tick_prv;
static u64 time_sec = 0;

static u64 SIMUL_TICKS = 0;
static u64 REFRESH_TICKS = 0;
static u64 SBLOL_ANIM_TICKS = 0;
static u64 GAME_ANIM_TICKS = 0;

static bool debug_mode = false;

static CJQ_GameState gamestate;
static C3D_RenderTarget* TOP_SCREEN;
static C3D_RenderTarget* BOT_SCREEN;


void GS_Init(void)
{
    // Initialize screens
	GS_SetTimeSec(0);
	GS_SetGameState(MODE_MENU);
	
	// Set tick tracking states
    GS_UpdTickNow();
	GS_SetFrameTickPrv(GS_GetTickNow());
	SIMUL_TICKS = GS_GetTickDelay_hz(SIMUL_RATE_HZ);
	REFRESH_TICKS = GS_GetTickDelay_hz(REFRESH_RATE_HZ);
    SBLOL_ANIM_TICKS = GS_GetTickDelay_hz(SBLOL_RATE_HZ);
    GAME_ANIM_TICKS = GS_GetTickDelay_hz(GAME_ANIM_HZ);
    GS_ResetFrameTickNet();

    // Create screens
    TOP_SCREEN = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	BOT_SCREEN = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
}


void GS_DelayTicks(size_t tick_delay)
{
    GS_SetFrameTickPrv(GS_GetTickNow());
    while(!GS_CheckDelayTimer(frame_tick_prv, tick_delay)) {
        GS_UpdTickNow();
    }
}


void GS_IncrTimeSec(void)
{
    if (GS_CheckDelayTimer(timer_tick_prv, GS_GetTickDelay_ms(1000))) {
        time_sec += 1;
        timer_tick_prv = svcGetSystemTick();
    }
}


u64 GS_GetTickDelay_ms(u64 delay_ms)
{
	u64 delay_ticks = (delay_ms * SYSCLOCK_ARM11) / 1000;
	return delay_ticks;
}


u64 GS_GetTickDelay_hz(u64 delay_hz)
{
	u64 delay_ticks = SYSCLOCK_ARM11 / delay_hz;
	return delay_ticks;
}


bool GS_CheckDelayTimer(u64 tick_ref, u64 delay_ticks)
{
	bool result;
	if  (GS_GetTickNow() >= tick_ref) {
		result = (GS_GetTickNow() - tick_ref) > delay_ticks;
	} else {
		result = (GS_GetTickNow() + (ULLONG_MAX - tick_ref)) > delay_ticks;
	}
	return result;
}


void GS_SetGameState(CJQ_GameState new_gamestate)
{
    gamestate = new_gamestate;
}


CJQ_GameState GS_GetGameState(void)
{
    return gamestate;
}


bool GS_GetIsDebugMode(void)
{
    return debug_mode;
}


C3D_RenderTarget* GS_GetTopScreen(void)
{
    return TOP_SCREEN;
}


C3D_RenderTarget* GS_GetBotScreen(void)
{
    return BOT_SCREEN;
}


void GS_ResetFrameTickNet(void)
{
    frame_tick_net = 0;
}


void GS_IncrFrameTickNet(u64 tick_delta)
{
    frame_tick_net += tick_delta;
}


void GS_DecrFrameTickNet(u64 tick_delta)
{
    frame_tick_net -= tick_delta;
}


u64 GS_GetFrameTickNet(void)
{
    return frame_tick_net;
}


u64 GS_GetTickFrame(void)
{
    if (GS_GetTickNow() >= GS_GetFrameTickPrv()) {
        return GS_GetTickNow() - GS_GetFrameTickPrv();
    } else {
        return GS_GetTickNow() + (ULLONG_MAX - GS_GetFrameTickPrv());
    }
}


void GS_SetTimeSec(u64 time)
{
    time_sec = time;
}


u64 GS_GetTimeSec(void)
{
    return time_sec;
}


void GS_SetFrameTickPrv(u64 tick)
{
    frame_tick_prv = tick;
}


u64 GS_GetFrameTickPrv(void)
{
    return frame_tick_prv;
}


void GS_UpdTickNow(void)
{
    tick_now = svcGetSystemTick();
}


u64 GS_GetTickNow(void)
{
    return tick_now;
}


u64 GS_GetSimulTicks(void)
{
    return SIMUL_TICKS;
}


u64 GS_GetRefreshTicks(void)
{
    return REFRESH_TICKS;
}


u64 GS_GetSBLOLAnimTicks(void)
{
    return SBLOL_ANIM_TICKS;
}


u64 GS_GetGameAnimTicks(void)
{
    return GAME_ANIM_TICKS;
}