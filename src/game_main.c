// JADEFRACTURE
// game_main.c

#include "i_event.h"
#include "i_system.h"
#include "i_video.h"

#include "game_defs.h"
#include "game_gfx.h"
#include "game_input.h"
#include "game_main.h"
#include "game_object.h"
#include "game_video.h"

bool game_quit = false;

// Game startup / main function.
void gameMain(void)
{
	printf("Initializing video...\n");
	V_Init();
	
	printf("Loading palette...\n");
	V_LoadPalette();
	
	printf("Starting graphics backend...\n");
	I_StartupGraphics();
	
	printf("Setting default controls...\n");
	G_DefaultControls();
	
	GFX_InitGFX();
	OBJ_InitObjects();
	
	for (int i = 0; i < 5000; i++)
		OBJ_CreateObject(rand()%(vid.width-24) << SUBPIXEL_SHIFT, rand()%(vid.height-32) << SUBPIXEL_SHIFT, OBJ_MARIL);
}

// The main game loop.
void gameLoop(void)
{
	uint32_t game_tick, elapsed_tick;
	uint32_t old_tick = I_GetTicks();
	uint32_t render_tick = 0;
	
	while (!game_quit) {
		game_tick = I_GetTicks();
		elapsed_tick = game_tick - old_tick;
		old_tick = game_tick;
		
		I_PollEvents();

		if (game_quit)
			break;
		
		if (elapsed_tick == 0)
		{
			I_Sleep(1);
			continue;
		}
		
		#if defined(__NDS__)
		// Force the game to run a tick on NDS otherwise it cries.
		gameRunStuff(1);
		#else
		gameRunStuff(elapsed_tick);
		#endif
		
		if (game_tick > render_tick)
		{
			render_tick = game_tick;
			
			I_PushGraphics();
		}
		
		V_ClearScreen();
	}
}

void gameRunStuff(uint32_t elapsed)
{
	if (elapsed > 4)
		elapsed = 1;
	
	while (elapsed--)
	{	
		OBJ_RunObjects();
	}
}