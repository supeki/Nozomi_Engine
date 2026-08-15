// Nozomi Engine
// GBC backend
// gbc_system.c

#include "gbc_main.h"

#include "../../i_system.h"
#include "../../game_defs.h"

uint32_t I_GetTicks(void)
{
	static uint32_t base_tick = 0;
	uint32_t ticks = TIMA_REG;
	
	if (!base_tick)
		base_tick = ticks;
	
	ticks -= base_tick;
	ticks = (ticks*FRAMERATE);
	ticks = (ticks/1000);
	
	return ticks;
}

uint32_t I_GetTime(void)
{
	return TIMA_REG;
}

void I_Sleep(uint32_t ms)
{
	delay(ms);
}

void I_printf (const char *text, ...)
{
}

void I_Error (const char *error, ...)
{
    I_Sleep(5000);
	exit(-1);
}
