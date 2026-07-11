// Nozomi Engine
// SDL2 backend
// sdl_event.c

#include <SDL2/SDL.h>

#include "sdl_main.h"
#include "sdl_input.h"

#include "../../i_event.h"
#include "../../game_main.h"

void I_PollEvents(void)
{
	SDL_Event event;
    while (SDL_PollEvent(&event)) {
		I_UpdateControls(event);
		
		switch (event.type)
		{
			case SDL_QUIT:
				game_quit = true;
				break;
			case SDL_WINDOWEVENT:
				I_UpdateWindow(event);
				break;
		}
    }
}
