// Nozomi Engine
// SDL2 backend
// sdl_main.c

#include <SDL2/SDL.h>

#if defined(PSP)
#include <pspdebug.h>
#endif

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
	
	I_printf("Initializing SDL!\n");
	
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_GAMECONTROLLER) < 0)
	{
		I_Error("Could not initialize SDL: %s.\n", SDL_GetError());
	}
	
	I_printf("Successfully initialized SDL!\n");
	
	#if defined(PSP)
	pspDebugScreenInit();
	chdir("ms0:/PSP/GAME/JADEFRACTURE");
	#endif
	
	gameMain();
	
	I_printf("Starting main game loop!\n");
		
	gameLoop();
	
	I_printf("Quitting SDL!\n");
	
	SDL_Quit();
	I_printf("Quitting %s...\n", GAME_NAME);
	
	exit(0);
}
