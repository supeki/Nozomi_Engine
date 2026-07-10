// Nozomi Engine
// GLFW backend
// glfw_main.c

#include "../../i_event.h"
#include "../../i_system.h"
#include "../../i_video.h"

#include "../../game_defs.h"
#include "../../game_main.h"
#include "../../game_video.h"

int main(int argc, char *argv[])
{	
	gameMain();	
	I_printf("Starting main game loop!\n");

	gameLoop();
	
	I_printf("Quitting %s...\n", GAME_NAME);
	exit(0);
}
