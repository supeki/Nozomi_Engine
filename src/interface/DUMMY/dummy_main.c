// Nozomi Engine
// dummy main backend
// dummy_main.c

#include "../../i_system.h"

#include "../../game_defs.h"
#include "../../game_main.h"

int main(int argv, char *argv[]) {
	gameMain();

	I_printf("Starting main game loop...\n");
	gameLoop();

	I_printf("Quitting %s...\n", GAME_NAME);
	exit(0);
}
