// Nozomi Engine
// NDS backend
// nds_event.c

#include <nds.h>

#include "../../i_event.h"

#include "../../game_main.h"
#include "../../game_input.h"

uint16_t old_mouseposx = 0, old_mouseposy = 0;

void I_HandleInput(void)
{
	int c;
	uint32_t keys;
	
	scanKeys();
	keys = keysHeld();
	for (c = 0; c < NUMCONTROLS; c++)
		if (keys & gamecontrolbinds[0][c][1]) // holy fucking shit
			gamecontrols[0][c]++;

	if (keys & KEY_TOUCH) {
		touchPosition touchdata;
		touchRead(&touchdata);

		mousecontrols[0][MOUSE_LBUTTON]++;
		mouseaxis[0][MOUSE_POSX] = touchdata.px;
		mouseaxis[0][MOUSE_POSY] = touchdata.py;
		mouseaxis[0][MOUSE_INPUTX] = touchdata.px - old_mouseposx;
		mouseaxis[0][MOUSE_INPUTY] = touchdata.py - old_mouseposy;
		old_mouseposx = touchdata.px;
		old_mouseposy = touchdata.py;
	}
			
	keys = keysUp();
	for (c = 0; c < NUMCONTROLS; c++)
		if (keys & gamecontrolbinds[0][c][1]) // holy fucking shit
			gamecontrols[0][c] = 0;

	if (keys & KEY_TOUCH)
		mousecontrols[0][MOUSE_LBUTTON] = 0;
}

void I_PollEvents(void)
{
	I_HandleInput();
}
