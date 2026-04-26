// JADEFRACTURE
// SDL2 backend
// sdl_system.c

#include <SDL2/SDL.h>

#if defined(PSP)
#include <pspdebug.h>
#endif

#include "sdl_main.h"

#include "../../i_system.h"
#include "../../game_defs.h"

uint32_t I_GetTicks(void)
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

uint32_t I_GetTime(void)
{
	return SDL_GetTicks();
}

void I_Sleep(uint32_t ms)
{
	SDL_Delay(ms);
}

void I_printf (char *text, ...)
{
    va_list argptr;
    char txt[2048];
    
    va_start(argptr,text);
    vsprintf(txt, text, argptr);
    va_end(argptr);    
	
	#if defined(PSP)
	pspDebugScreenPrintf(txt);
	#else
	printf(txt);
	#endif
}

void I_Error (char *error, ...)
{
    va_list argptr;
    char txt[2048];
    
    va_start(argptr,error);
    vsprintf(txt, error, argptr);
    va_end(argptr);    
	
	I_printf("JADEFRACTURE Error: %s", txt);
	SDL_ShowSimpleMessageBox(0, "JADEFRACTURE Error", txt, sdlWnd);
	
	#if !defined(PSP)
    exit(-1);
	#endif
}
