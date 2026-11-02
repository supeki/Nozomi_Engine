// Nozomi Engine
// X11 backend
// x11_system.c

#include "x11_main.h"

#include "../../i_system.h"

#include "../../game_defs.h"

uint32_t I_GetTicks(void)
{
	struct timeval tp;
	struct timezone tz;
	static uint32_t base_tick = 0;

	gettimeofday(&tp, &tz);

	if (!base_tick)
		base_tick = tp.tv_sec;

	return (tp.tv_sec - base_tick)*FRAMERATE + tp.tv_usec*FRAMERATE/1000000;
}

uint32_t I_GetTime(void)
{
	struct timeval tp;
	struct timezone tz;

	gettimeofday(&tp, &tz);
	return tp.tv_sec * 1000 + tp.tv_usec/1000;
}

void I_Sleep(uint32_t ms)
{
	usleep(ms * 1000);
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

	I_printf("Nozomi Engine Error: %s\n", txt);
	I_Sleep(5000);
	exit(-1);
}
