// Nozomi Engine
// dummy system backend
// dummy_system.c

#include "../../i_system.h"

#include "../../game_defs.h"

uint32_t bt;

uint32_t I_GetTicks(void)
{
	static uint32_t base_tick = 0;
	uint32_t ticks = I_GetTime();

	if (!base_tick)
		base_tick = ticks;

	ticks -= base_tick;
	ticks = (ticks*FRAMERATE);
	ticks = (ticks/1000);

	return ticks;
}

uint32_t I_GetTime(void)
{
	bt++;
	return bt;
}

void I_Sleep(uint32_t ms)
{
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
	char txt[2048];

	va_start(argptr,error);
	vsprintf(txt, error, argptr);
	va_end(argptr);

	printf("Nozomi Engine Error: %s\n", txt);
	I_Sleep(5000);
	exit(-1);
}
