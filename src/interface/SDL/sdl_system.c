// JADEFRACTURE
// SDL2 backend
// sdl_system.c

#include <SDL.h>

#include "sdl_main.h"

#include "../../i_system.h"
#include "../../game_defs.h"

int I_GetTicks(void)
{
	static uint32_t base_tick = 0;
	uint32_t ticks = SDL_GetTicks();
	
	if (!base_tick)
		base_tick = ticks;
	
	ticks -= base_tick;
	ticks = (ticks*FRAMERATE);
	ticks = (ticks/1000);
	
	return ticks;
}

int I_GetTime(void)
{
	return SDL_GetTicks();
}

void I_Sleep(int ms)
{
	SDL_Delay(ms);
}

void I_Error (char *error, ...)
{
    va_list argptr;
    char txt[512];
    
    va_start(argptr,error);
    vsprintf(txt, error, argptr);
    va_end(argptr);    
	
    SDL_ShowSimpleMessageBox(0, "JADEFRACTURE Error", txt, sdlWnd);
    exit(-1);
}