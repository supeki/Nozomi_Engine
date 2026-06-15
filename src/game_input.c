// Nozomi Engine
// game_input.c

#if defined(__NDS__)
#include <nds.h>
#elif defined(GLFW)
#include <GLFW/glfw3.h>
#elif defined(SDL)
#include <SDL2/SDL.h>
#endif

#include "game_input.h"

uint32_t gamecontrols[NUMCONTROLS];
uint32_t gamecontrolbinds[NUMCONTROLS][2];

bool G_ControlDown(uint8_t control, bool pressed)
{
	if (control > NUMCONTROLS-1)
		return false;
	
	if (pressed)
		return (gamecontrols[control] == 1);
	else
		return (gamecontrols[control] > 0);
}

void G_DefaultControls(void)
{
	int i;
	
	for (i = 0; i < NUMCONTROLS; i++)
	{
		gamecontrolbinds[i][0] = -1;
		gamecontrolbinds[i][1] = -1;
	}
	
#if defined(__NDS__)
	gamecontrolbinds[CON_UP][1] = KEY_UP;
	gamecontrolbinds[CON_DOWN][1] = KEY_DOWN;
	gamecontrolbinds[CON_LEFT][1] = KEY_LEFT;
	gamecontrolbinds[CON_RIGHT][1] = KEY_RIGHT;
	gamecontrolbinds[CON_A][1] = KEY_A;
	gamecontrolbinds[CON_B][1] = KEY_Y;
	gamecontrolbinds[CON_C][1] = KEY_B;
	gamecontrolbinds[CON_START][1] = KEY_START;
	gamecontrolbinds[CON_SELECT][1] = KEY_SELECT;
	gamecontrolbinds[CON_X][1] = KEY_L;
	gamecontrolbinds[CON_Y][1] = KEY_R;
	gamecontrolbinds[CON_Z][1] = KEY_X;
	gamecontrolbinds[CON_CONFIRM][1] = KEY_A;
	gamecontrolbinds[CON_BACK][1] = KEY_B;
#elif defined(GLFW)
	gamecontrolbinds[CON_UP][0] = GLFW_KEY_UP;
	gamecontrolbinds[CON_DOWN][0] = GLFW_KEY_DOWN;
	gamecontrolbinds[CON_LEFT][0] = GLFW_KEY_LEFT;
	gamecontrolbinds[CON_RIGHT][0] = GLFW_KEY_RIGHT;
	gamecontrolbinds[CON_A][0] = GLFW_KEY_Z;
	gamecontrolbinds[CON_B][0] = GLFW_KEY_X;
	gamecontrolbinds[CON_C][0] = GLFW_KEY_Z;
	gamecontrolbinds[CON_START][0] = GLFW_KEY_ESCAPE;
	gamecontrolbinds[CON_SELECT][0] = GLFW_KEY_RIGHT_SHIFT;
	gamecontrolbinds[CON_X][0] = GLFW_KEY_A;
	gamecontrolbinds[CON_Y][0] = GLFW_KEY_S;
	gamecontrolbinds[CON_Z][0] = GLFW_KEY_D;
	gamecontrolbinds[CON_CONFIRM][0] = GLFW_KEY_Z;
	gamecontrolbinds[CON_BACK][0] = GLFW_KEY_X;
#elif defined(SDL)
	gamecontrolbinds[CON_UP][0] = SDLK_UP;
	gamecontrolbinds[CON_DOWN][0] = SDLK_DOWN;
	gamecontrolbinds[CON_LEFT][0] = SDLK_LEFT;
	gamecontrolbinds[CON_RIGHT][0] = SDLK_RIGHT;
	gamecontrolbinds[CON_A][0] = 'z';
	gamecontrolbinds[CON_B][0] = 'x';
	gamecontrolbinds[CON_C][0] = 'c';
	gamecontrolbinds[CON_START][0] = SDLK_RETURN;
	gamecontrolbinds[CON_SELECT][0] = SDLK_RSHIFT;
	gamecontrolbinds[CON_X][0] = 'a';
	gamecontrolbinds[CON_Y][0] = 's';
	gamecontrolbinds[CON_Z][0] = 'd';
	gamecontrolbinds[CON_CONFIRM][0] = 'z';
	gamecontrolbinds[CON_BACK][0] = 'x';
	gamecontrolbinds[CON_UP][1] = SDL_CONTROLLER_BUTTON_DPAD_UP;
	gamecontrolbinds[CON_DOWN][1] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	gamecontrolbinds[CON_LEFT][1] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	gamecontrolbinds[CON_RIGHT][1] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
	gamecontrolbinds[CON_A][1] = SDL_CONTROLLER_BUTTON_B;
	gamecontrolbinds[CON_B][1] = SDL_CONTROLLER_BUTTON_X;
	gamecontrolbinds[CON_C][1] = SDL_CONTROLLER_BUTTON_A;
	gamecontrolbinds[CON_START][1] = SDL_CONTROLLER_BUTTON_START;
	gamecontrolbinds[CON_SELECT][1] = SDL_CONTROLLER_BUTTON_BACK;
	gamecontrolbinds[CON_X][1] = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
	gamecontrolbinds[CON_Y][1] = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
	gamecontrolbinds[CON_Z][1] = SDL_CONTROLLER_BUTTON_Y;
	gamecontrolbinds[CON_CONFIRM][1] = SDL_CONTROLLER_BUTTON_A;
	gamecontrolbinds[CON_BACK][1] = SDL_CONTROLLER_BUTTON_B;
#endif
}
