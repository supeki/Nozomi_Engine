// JADEFRACTURE
// game_input.c

#if defined(__NDS__)
#include <nds.h>
#elif defined(SDL)
#include <SDL.h>
#endif

#include "game_input.h"

uint32_t gamecontrols[NUMCONTROLS];
uint32_t gamecontrolbinds[NUMCONTROLS][2];

bool G_ControlDown(uint8_t control, bool pressed)
{
	if (control > NUMCONTROLS-1)
		return false;
	
	if (gamecontrols[CON_UP] > gamecontrols[CON_DOWN] && gamecontrols[CON_DOWN] > 0)
		gamecontrols[CON_DOWN] = 0;
	else if (gamecontrols[CON_DOWN] > gamecontrols[CON_UP] && gamecontrols[CON_UP] > 0)
		gamecontrols[CON_UP] = 0;
	
	if (gamecontrols[CON_LEFT] > gamecontrols[CON_RIGHT] && gamecontrols[CON_RIGHT] > 0)
		gamecontrols[CON_RIGHT] = 0;
	else if (gamecontrols[CON_RIGHT] > gamecontrols[CON_LEFT] && gamecontrols[CON_LEFT] > 0)
		gamecontrols[CON_LEFT] = 0;
	
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
	#endif
}