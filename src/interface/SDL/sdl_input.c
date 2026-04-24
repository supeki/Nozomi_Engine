// JADEFRACTURE
// SDL2 backend
// sdl_input.c

#include <SDL.h>

#include "sdl_input.h"

#include "../../i_system.h"
#include "../../game_input.h"

// handled differently for every backend...
void I_UpdateControls(SDL_Event event)
{
	int c;
	
	switch (event.type) {
		case SDL_KEYDOWN:
			for (c = CON_UP; c < NUMCONTROLS; c++)
				if (gamecontrolbinds[c][0] == event.key.keysym.sym) // holy fucking shit
					gamecontrols[c] = 1;
			break;
		case SDL_KEYUP:
			for (c = CON_UP; c < NUMCONTROLS; c++)
				if (gamecontrolbinds[c][0] == event.key.keysym.sym) // holy fucking shit
					gamecontrols[c] = 0;
			break;
	}
}
