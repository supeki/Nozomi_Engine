// JADEFRACTURE
// game_main.c

#include "i_event.h"
#include "i_system.h"
#include "i_video.h"

#include "game_defs.h"
#include "game_main.h"
#include "game_video.h"

bool game_quit = false;
static int sand_x = 0;
static int sand_y = 0;
static int k = 0;

// Game startup / main function.
void gameMain(void)
{
	printf("Initializing video...\n");
	V_Init();
	
	printf("Starting graphics backend...\n");
	I_StartupGraphics();
}

// The main game loop.
void gameLoop(void)
{
	int old_tick = I_GetTicks();
	int render_tick = 0;
	
	while (!game_quit) {
		int game_tick = I_GetTicks();
		int elapsed_tick = game_tick - old_tick;
		old_tick = game_tick;
		
		I_PollEvents();

		if (game_quit)
			break;
		
		if (elapsed_tick == 0)
		{
			I_Sleep(1);
			continue;
		}
		
		gameRunTicks(elapsed_tick);
		
		if (game_tick > render_tick)
		{
			render_tick = game_tick;
			
			V_DrawDot(sand_x, sand_y, (int)(k % (sand_x+1)));
			I_PushGraphics();
		}
	}
}

void gameRunTicks(int elapsed)
{
	while (elapsed-- > 0)
	{
		// run stuff! :3 Nozomi
		sand_x = k % (vid.width+1);
		
		while (sand_x >= vid.width)
		{
			sand_y++;
			sand_x -= vid.width;
		}
		
		k++;
	}
}