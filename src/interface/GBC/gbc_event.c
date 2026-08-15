// Nozomi Engine
// GBC backend
// gbc_event.c

#include "gbc_main.h"

#include "../../i_event.h"

#include "../../game_main.h"
#include "../../game_input.h"

void I_PollEvents(void)
{
	int c;
	uint8_t keys;
	
	keys = joypad();
	for (c = 0; c < NUMCONTROLS; c++)
		if (keys & gamecontrolbinds[0][c][1]) // holy fucking shit
			gamecontrols[0][c]++;
}
