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

uint32_t gamecontrols[MAX_PLAYERS][NUMCONTROLS];
int16_t  gameaxis[MAX_PLAYERS][NUMAXIS];
uint32_t gamecontrolbinds[MAX_PLAYERS][NUMCONTROLS][2];

bool G_ControlDown(uint8_t player, uint8_t control, bool pressed)
{
	if (control > NUMCONTROLS-1)
		return false;
	
	if (gamecontrols[player][control] > 0xFFFFFFFE)
		gamecontrols[player][control] = 0xFFFFFFFF;
	
	if (pressed)
		return (gamecontrols[player][control] == 1);
	else
		return (gamecontrols[player][control] > 0);
}

int16_t G_PlayerAxis(uint8_t player, uint8_t axis)
{
	if (axis > NUMAXIS-1)
		return 0;
	
	if (gameaxis[player][axis] > 0xFFFF)
		gameaxis[player][axis] = 0xFFFF;
	if (gameaxis[player][axis] < -0xFFFF)
		gameaxis[player][axis] = -0xFFFF;
	
	if (abs(gameaxis[player][axis]) <= 0x1FF)
		return 0;
	
	return gameaxis[player][axis];
}

void G_DefaultControls(void)
{
	int i, j;
	
	for (j = 0; j < MAX_PLAYERS; j++)
	{
		for (i = 0; i < NUMCONTROLS; i++)
		{
			gamecontrols[j][i] = 0;
			gamecontrolbinds[j][i][0] = -1;
			gamecontrolbinds[j][i][1] = -1;
		}
		
		for (i = 0; i < NUMAXIS; i++)
			gameaxis[j][i] = 0;
	
	#if defined(__NDS__)
		gamecontrolbinds[j][CON_UP][1] = KEY_UP;
		gamecontrolbinds[j][CON_DOWN][1] = KEY_DOWN;
		gamecontrolbinds[j][CON_LEFT][1] = KEY_LEFT;
		gamecontrolbinds[j][CON_RIGHT][1] = KEY_RIGHT;
		gamecontrolbinds[j][CON_A][1] = KEY_A;
		gamecontrolbinds[j][CON_B][1] = KEY_Y;
		gamecontrolbinds[j][CON_C][1] = KEY_B;
		gamecontrolbinds[j][CON_START][1] = KEY_START;
		gamecontrolbinds[j][CON_SELECT][1] = KEY_SELECT;
		gamecontrolbinds[j][CON_X][1] = KEY_L;
		gamecontrolbinds[j][CON_Y][1] = KEY_R;
		gamecontrolbinds[j][CON_Z][1] = KEY_X;
		gamecontrolbinds[j][CON_CONFIRM][1] = KEY_A;
		gamecontrolbinds[j][CON_BACK][1] = KEY_B;
	#elif defined(GLFW)
		gamecontrolbinds[j][CON_UP][0] = GLFW_KEY_UP;
		gamecontrolbinds[j][CON_DOWN][0] = GLFW_KEY_DOWN;
		gamecontrolbinds[j][CON_LEFT][0] = GLFW_KEY_LEFT;
		gamecontrolbinds[j][CON_RIGHT][0] = GLFW_KEY_RIGHT;
		gamecontrolbinds[j][CON_A][0] = GLFW_KEY_Z;
		gamecontrolbinds[j][CON_B][0] = GLFW_KEY_X;
		gamecontrolbinds[j][CON_C][0] = GLFW_KEY_C;
		gamecontrolbinds[j][CON_START][0] = GLFW_KEY_ENTER;
		gamecontrolbinds[j][CON_SELECT][0] = GLFW_KEY_RIGHT_SHIFT;
		gamecontrolbinds[j][CON_X][0] = GLFW_KEY_A;
		gamecontrolbinds[j][CON_Y][0] = GLFW_KEY_S;
		gamecontrolbinds[j][CON_Z][0] = GLFW_KEY_D;
		gamecontrolbinds[j][CON_CONFIRM][0] = GLFW_KEY_Z;
		gamecontrolbinds[j][CON_BACK][0] = GLFW_KEY_X;
	#elif defined(SDL)
		gamecontrolbinds[j][CON_UP][0] = SDLK_UP;
		gamecontrolbinds[j][CON_DOWN][0] = SDLK_DOWN;
		gamecontrolbinds[j][CON_LEFT][0] = SDLK_LEFT;
		gamecontrolbinds[j][CON_RIGHT][0] = SDLK_RIGHT;
		gamecontrolbinds[j][CON_A][0] = 'z';
		gamecontrolbinds[j][CON_B][0] = 'x';
		gamecontrolbinds[j][CON_C][0] = 'c';
		gamecontrolbinds[j][CON_START][0] = SDLK_RETURN;
		gamecontrolbinds[j][CON_SELECT][0] = SDLK_RSHIFT;
		gamecontrolbinds[j][CON_X][0] = 'a';
		gamecontrolbinds[j][CON_Y][0] = 's';
		gamecontrolbinds[j][CON_Z][0] = 'd';
		gamecontrolbinds[j][CON_CONFIRM][0] = 'z';
		gamecontrolbinds[j][CON_BACK][0] = 'x';
		gamecontrolbinds[j][CON_UP][1] = SDL_CONTROLLER_BUTTON_DPAD_UP;
		gamecontrolbinds[j][CON_DOWN][1] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
		gamecontrolbinds[j][CON_LEFT][1] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
		gamecontrolbinds[j][CON_RIGHT][1] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
		gamecontrolbinds[j][CON_A][1] = SDL_CONTROLLER_BUTTON_X;
		gamecontrolbinds[j][CON_B][1] = SDL_CONTROLLER_BUTTON_A;
		gamecontrolbinds[j][CON_C][1] = SDL_CONTROLLER_BUTTON_B;
		gamecontrolbinds[j][CON_START][1] = SDL_CONTROLLER_BUTTON_START;
		gamecontrolbinds[j][CON_SELECT][1] = SDL_CONTROLLER_BUTTON_BACK;
		gamecontrolbinds[j][CON_X][1] = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
		gamecontrolbinds[j][CON_Y][1] = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
		gamecontrolbinds[j][CON_Z][1] = SDL_CONTROLLER_BUTTON_Y;
		gamecontrolbinds[j][CON_CONFIRM][1] = SDL_CONTROLLER_BUTTON_A;
		gamecontrolbinds[j][CON_BACK][1] = SDL_CONTROLLER_BUTTON_B;
	#endif
	}
}
