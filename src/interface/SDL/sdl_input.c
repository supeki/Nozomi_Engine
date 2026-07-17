// Nozomi Engine
// SDL2 backend
// sdl_input.c

#include <SDL2/SDL.h>

#include "sdl_input.h"

#include "../../i_system.h"
#include "../../game_input.h"

static int player_controllers[MAX_PLAYERS] = {-1, -1, -1, -1};
static SDL_GameController *player_gamecontroller[MAX_PLAYERS];

// handled differently for every backend...
void I_UpdateControls(SDL_Event event)
{
	int c, j, pl;
	
	switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.repeat)
				break;
			
			for (j = 0; j < MAX_PLAYERS; j++)
				for (c = CON_UP; c < NUMCONTROLS; c++)
					if (gamecontrolbinds[j][c][0] == event.key.keysym.sym) // holy fucking shit
						gamecontrols[j][c] = 1;
			break;
		case SDL_KEYUP:
			for (j = 0; j < MAX_PLAYERS; j++)
				for (c = CON_UP; c < NUMCONTROLS; c++)
					if (gamecontrolbinds[j][c][0] == event.key.keysym.sym) // holy fucking shit
						gamecontrols[j][c] = 0;
			break;
			
		// Controllers!
		case SDL_CONTROLLERDEVICEADDED:
		case SDL_JOYDEVICEADDED:
			SDL_GameController *gc;
			if (SDL_IsGameController(event.cdevice.which))
				gc = SDL_GameControllerOpen(event.cdevice.which);
			if (SDL_IsGameController(event.jdevice.which))
				gc = SDL_GameControllerOpen(event.jdevice.which);
			
			for (j = 0; j < MAX_PLAYERS; j++)
				if (player_controllers[j] == -1) {
					player_controllers[j] = event.cdevice.which;
					player_gamecontroller[j] = gc;
				}
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			for (j = 0; j < MAX_PLAYERS; j++)
				if (player_controllers[j] == event.cdevice.which) {
					player_controllers[j] = -1;
					player_gamecontroller[j] = NULL;
				}
		case SDL_CONTROLLERBUTTONDOWN:
			pl = -1;
			
			for (j = 0; j < MAX_PLAYERS; j++)
				if (player_controllers[j] == event.cdevice.which)
					pl = event.cdevice.which;
				
			if (pl == -1)
				break;
		
			for (c = CON_UP; c < NUMCONTROLS; c++)
				if (gamecontrolbinds[pl][c][1] == event.cbutton.button) // holy fucking shit
					gamecontrols[pl][c] = 1;
			break;
		case SDL_CONTROLLERBUTTONUP:
			pl = -1;
			
			for (j = 0; j < MAX_PLAYERS; j++)
				if (player_controllers[j] == event.cdevice.which)
					pl = event.cdevice.which;
				
			if (pl == -1)
				break;
		
			for (c = CON_UP; c < NUMCONTROLS; c++)
				if (gamecontrolbinds[pl][c][1] == event.cbutton.button) // holy fucking shit
					gamecontrols[pl][c] = 0;
			break;
	}
}

void I_UpdateControllerAxis(void)
{
	for (int j = 0; j < MAX_PLAYERS; j++)
		if (player_gamecontroller[j] != NULL) {
			gameaxis[j][AXIS_LEFTX] = SDL_GameControllerGetAxis(player_gamecontroller[j], SDL_CONTROLLER_AXIS_LEFTX);
			gameaxis[j][AXIS_LEFTY] = SDL_GameControllerGetAxis(player_gamecontroller[j], SDL_CONTROLLER_AXIS_LEFTY);
			gameaxis[j][AXIS_RIGHTX] = SDL_GameControllerGetAxis(player_gamecontroller[j], SDL_CONTROLLER_AXIS_RIGHTX);
			gameaxis[j][AXIS_RIGHTY] = SDL_GameControllerGetAxis(player_gamecontroller[j], SDL_CONTROLLER_AXIS_RIGHTY);
		}
}