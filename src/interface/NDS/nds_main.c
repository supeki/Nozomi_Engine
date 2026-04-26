// JADEFRACTURE
// NDS backend
// nds_main.c

#include <nds.h>
#include <filesystem.h>

#include "nds_main.h"

#include "../../i_event.h"
#include "../../i_system.h"
#include "../../i_video.h"

#include "../../game_defs.h"
#include "../../game_main.h"
#include "../../game_video.h"

int main(int argc, char *argv[])
{
	consoleDemoInit();
	defaultExceptionHandler();
	
	TIMER0_DATA=0;
	TIMER1_DATA=0;
	TIMER0_CR=TIMER_DIV_1024 | TIMER_ENABLE;
	TIMER1_CR=TIMER_CASCADE | TIMER_ENABLE;
	
	I_printf("Initializing NitroFS!\n");
	if (!nitroFSInit(NULL))
		libndsCrash("NitroFS failed to initialize!\nCannot proceed!");
	
	gameMain();
	
	I_printf("Starting main game loop!\n");
	gameLoop();
	
	I_printf("Quitting JADEFRACTURE...\n");
	
	exit(0);
}
