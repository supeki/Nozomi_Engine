// Nozomi Engine
// GLFW backend
// glfw_system.c

#include <GLFW/glfw3.h>
#include <unistd.h>

#include "../../i_system.h"
#include "../../game_defs.h"

uint32_t I_GetTicks(void)
{
	static double base_time = 0.0;
	double time = glfwGetTime();

	if (base_time == 0.0)
		base_time = time;

	time -= base_time;

	return (uint32_t)(time * FRAMERATE);
}

uint32_t I_GetTime(void)
{
	return (uint32_t)(glfwGetTime() * 1000.0);
}

// may not work on windows!
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
