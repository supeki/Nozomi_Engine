// JADEFRACTURE
// SDL2 backend
// sdl_system.c

#include <SDL.h>

#include "../../i_system.h"

#include "../../game_defs.h"

int I_GetTicks(void)
{
	static Uint32 base_tick = 0;
	Uint32 ticks = SDL_GetTicks();
	
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