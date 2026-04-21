// JADEFRACTURE
// NDS backend
// nds_event.c

#include <nds.h>

#include "../../i_event.h"

#include "../../game_main.h"
#include "../../game_input.h"

void I_HandleInput(void)
{
	int c;
	uint32_t keys;
	
	scanKeys();
	keys = keysHeld();
	for (c = 0; c < NUMCONTROLS; c++)
		if (keys & gamecontrolbinds[c][1]) // holy fucking shit
			gamecontrols[c]++;
			
	keys = keysUp();
	for (c = 0; c < NUMCONTROLS; c++)
		if (keys & gamecontrolbinds[c][1]) // holy fucking shit
			gamecontrols[c] = 0;
}

void I_PollEvents(void)
{
	I_HandleInput();
}