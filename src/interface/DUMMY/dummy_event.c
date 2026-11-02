// Nozomi Engine
// dummy event backend
// dummy_event.c

#include "../../i_event.h"

#include "../../game_input.h"

void I_PollEvents(void)
{
	int j, c;

	for (j = 0; j < MAX_PLAYERS; j++)
		for (c = CON_UP; c < NUMCONTROLS; c++)
        		gamecontrols[j][c] = 0;

	mouseaxis[0][MOUSE_POSX] = 0;
	mouseaxis[0][MOUSE_POSY] = 0;
	mouseaxis[0][MOUSE_INPUTX] = 0;
	mouseaxis[0][MOUSE_INPUTY] = 0;
        mousecontrols[0][MOUSE_LBUTTON] = 0;
}
