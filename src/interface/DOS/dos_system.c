// Nozomi Engine
// DOS/DJGPP backend
// dos_system.c

#include "../../game_defs.h"
#include "../../i_system.h"

uint32_t timer_ticks;

uint32_t I_GetTicks(void)
{
	static uint32_t base_tick = 0;
	uint32_t ticks = timer_ticks;
	
	if (!base_tick)
		base_tick = ticks;
	
	ticks -= base_tick;
	ticks = (ticks*FRAMERATE);
	ticks = (ticks/1000);
	
	return ticks;
}

uint32_t I_GetTime(void)
{
	return timer_ticks;
}

void I_Sleep(uint32_t ms)
{
	uint32_t temp_time = 0;
    while (temp_time < ms)
    {
        temp_time++;
    }
}

void I_printf (const char *text, ...)
{
    va_list argptr;
    char txt[2048];
    
    va_start(argptr,text);
    vsprintf(txt, text, argptr);
    va_end(argptr);    

	printf(txt);
}
void I_Error (const char *error, ...)
{
    va_list argptr;
    char txt[512];
    
    va_start(argptr,error);
    vsprintf(txt, error, argptr);
    va_end(argptr);    
	
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    printf("Nozomi Engine Error:\n%s\n", txt);
    I_Sleep(5000);
	exit(-1);
}