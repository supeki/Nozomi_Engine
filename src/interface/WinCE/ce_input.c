// Nozomi Engine
// WinCE backend
// ce_input.c

#include "ce_input.h"

#include "../../i_system.h"
#include "../../game_input.h"

// handled differently for every backend...
/*void I_UpdateControls(SDL_Event event)
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
	}
}*/
