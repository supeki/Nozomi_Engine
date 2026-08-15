// Nozomi Engine
// WinCE backend
// ce_system.c

#include "ce_main.h"

#include "../../i_system.h"
#include "../../game_defs.h"

uint32_t I_GetTicks(void)
{
	static uint32_t base_tick = 0;
	uint32_t ticks;

	ticks = (uint32_t)GetTickCount();

	if (base_tick == 0)
		base_tick = ticks;

	ticks -= base_tick;

	return (ticks * FRAMERATE) / 1000;
}

uint32_t I_GetTime(void)
{
	return (uint32_t)GetTickCount();
}

void I_Sleep(uint32_t ms)
{
	Sleep(ms);
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

	{
		wchar_t buf[256];
		MultiByteToWideChar(0, 0, txt, strlen(txt), buf, strlen(txt));
		MessageBox(
			NULL,
			buf,
			L"Nozomi Engine Error",
			MB_OK | MB_ICONSTOP
		);
	}

    exit(-1);
}
