// JADEFRACTURE
// SDL2 backend
// sdl_main.c

#include <SDL.h>

#include "sdl_main.h"

#include "../../i_event.h"
#include "../../i_system.h"
#include "../../i_video.h"

#include "../../game_defs.h"
#include "../../game_main.h"
#include "../../game_video.h"

int main(int argc, char *argv[])
{
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
	
	printf("Initializing SDL!\n");
	
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0)
	{
		printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
	}
	
	printf("Successfully initialized SDL!\n");
	
	gameMain();
	BMPGFX("marilyn.bmp");
	
	gameLoop();
	
	printf("Quitting SDL!\n");
	
	SDL_Quit();
	printf("Quitting JADEFRACTURE...\n");
	
	exit(0);
}