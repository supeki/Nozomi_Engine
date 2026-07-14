// Nozomi Engine
// SDL2 backend
// sdl_input.c

#include <SDL2/SDL.h>

#include "sdl_input.h"

#include "../../i_system.h"
#include "../../game_input.h"

// handled differently for every backend...
void I_UpdateControls(SDL_Event event)
{
	int c;
	
	switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.repeat)
				break;
		
			for (c = CON_UP; c < NUMCONTROLS; c++)
				if (gamecontrolbinds[c][0] == event.key.keysym.sym) // holy fucking shit
					gamecontrols[c] = 1;
			break;
		case SDL_KEYUP:
			for (c = CON_UP; c < NUMCONTROLS; c++)
				if (gamecontrolbinds[c][0] == event.key.keysym.sym) // holy fucking shit
					gamecontrols[c] = 0;
			break;
			
		// Controllers!
		case SDL_CONTROLLERDEVICEADDED:
		case SDL_JOYDEVICEADDED:
			if (SDL_IsGameController(event.cdevice.which))
				SDL_GameControllerOpen(event.cdevice.which);
			if (SDL_IsGameController(event.jdevice.which))
				SDL_GameControllerOpen(event.jdevice.which);
			break;
		case SDL_CONTROLLERAXISMOTION:
			switch (event.caxis.axis) {
				case SDL_CONTROLLER_AXIS_LEFTX:
					if (event.caxis.value < -1024) { // temp hardcoded deadzones y'know
						gamecontrols[CON_LEFT] = 1;
						gamecontrols[CON_RIGHT] = 0;
					} else if (event.caxis.value > 1024) {
						gamecontrols[CON_LEFT] = 0;
						gamecontrols[CON_RIGHT] = 1;
					} else {
						gamecontrols[CON_LEFT] = 0;
						gamecontrols[CON_RIGHT] = 0;
					}
					
					break;
				case SDL_CONTROLLER_AXIS_LEFTY:
					if (event.caxis.value < -1024) { // temp hardcoded deadzones y'know
						gamecontrols[CON_UP] = 1;
						gamecontrols[CON_DOWN] = 0;
					} else if (event.caxis.value > 1024) {
						gamecontrols[CON_UP] = 0;
						gamecontrols[CON_DOWN] = 1;
					} else {
						gamecontrols[CON_UP] = 0;
						gamecontrols[CON_DOWN] = 0;
					}
					
					break;
			}
            break;
		case SDL_CONTROLLERBUTTONDOWN:
			for (c = CON_UP; c < NUMCONTROLS; c++)
				if (gamecontrolbinds[c][1] == event.cbutton.button) // holy fucking shit
					gamecontrols[c] = 1;
			break;
		case SDL_CONTROLLERBUTTONUP:
			for (c = CON_UP; c < NUMCONTROLS; c++)
				if (gamecontrolbinds[c][1] == event.cbutton.button) // holy fucking shit
					gamecontrols[c] = 0;
			break;
	}
}