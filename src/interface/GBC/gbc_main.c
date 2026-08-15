// Nozomi Engine
// GBC backend
// gbc_main.c

#include "gbc_main.h"

#include "../../i_event.h"
#include "../../i_system.h"
#include "../../i_video.h"

#include "../../game_defs.h"
#include "../../game_main.h"
#include "../../game_video.h"

int main(int argc, char *argv[])
{
	for (uint8_t i = 4; i != 0; i--)
        	vsync();

	DISPLAY_ON;

	if (_cpu != CGB_TYPE) {
		printf("Nozomi Engine only supports\nGame Boy Color.\n");
		while(1)
		{
			vsync();
		}
	}
	
	gameMain();
	
	//I_printf("Starting main game loop...\n");
	gameLoop();
	
	while(1)
	{
		vsync();
	}

	//I_printf("Quitting %s...\n", GAME_NAME);
	
	exit(0);
}
