// JADEFRACTURE
// NDS backend
// nds_system.c

#include <nds.h>
#include <stdio.h>

#include "nds_main.h"

#include "../../i_system.h"
#include "../../game_defs.h"

#define timers2ms(tlow,thigh) ((tlow>>5)+(thigh<<11))

uint32_t I_GetTicks(void)
{
	static uint32_t base_tick = 0;
	uint32_t ticks = timers2ms(TIMER0_DATA, TIMER1_DATA);
	
	if (!base_tick)
		base_tick = ticks;
	
	ticks -= base_tick;
	ticks = (ticks*FRAMERATE);
	ticks = (ticks/1000);
	
	return ticks;
}

void Pause(uint32_t ms)
{
	u32 now;
	now=timers2ms(TIMER0_DATA, TIMER1_DATA);
	while((u32)timers2ms(TIMER0_DATA, TIMER1_DATA)<now+ms);
}

uint32_t I_GetTime(void)
{
	return timers2ms(TIMER0_DATA, TIMER1_DATA);
}

void I_Sleep(uint32_t ms)
{
	Pause(ms/1000);
}

void I_Error (char *error, ...)
{
    va_list argptr;
    char txt[512];
    
    va_start(argptr,error);
    vsprintf(txt, error, argptr);
    va_end(argptr);    
	
    printf("JADEFRACTURE Error:\n%s\n", txt);
    exit(-1);
}