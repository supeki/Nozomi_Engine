// JADEFRACTURE
// game_main.cpp

#include "i_event.h"
#include "i_video.h"

#include "game_defs.h"
#include "game_main.h"

bool game_quit = false;

// The main game loop.
void gameMain(void)
{
	while (!game_quit) {
		I_PollEvents();

		if (game_quit)
			break;
		
		I_PushGraphics();
	}
}