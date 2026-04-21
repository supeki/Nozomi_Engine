// JADEFRACTURE
// game_main.c

#include "i_event.h"
#include "i_system.h"
#include "i_video.h"

#include "game_defs.h"
#include "game_gfx.h"
#include "game_main.h"
#include "game_video.h"

bool game_quit = false;
gfx_t test;

// Game startup / main function.
void gameMain(void)
{
	printf("Initializing video...\n");
	V_Init();
	
	printf("Loading palette...\n");
	V_LoadPalette();
	
	printf("Starting graphics backend...\n");
	I_StartupGraphics();
	
	test = GFX_LoadGFX("marilyn.bmp.gfx");
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
			
			I_PushGraphics();
		}
	}
}

void gameRunTicks(int elapsed)
{
	while (elapsed-- > 0)
	{
		// Write some logic here later :3
		
		// o'tayyy doing this for save
		for (int y = 0; y < vid.height; y++)
			for (int x = 0; x < vid.width; x++)
				V_DrawDot(x, y, 0);
			
		V_DrawCroppedSprite(test, 0, 0, 24, 32, 24, 32);
	}
}