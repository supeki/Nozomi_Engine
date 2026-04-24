// JADEFRACTURE
// SDL2 backend
// sdl_event.c

#include <SDL.h>

#include "sdl_input.h"

#include "../../i_event.h"
#include "../../game_main.h"

void I_PollEvents(void)
{
	SDL_Event event;
    while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				I_UpdateControls(event);
				break;
			case SDL_QUIT:
				game_quit = true;
				break;
		}
    }
}
