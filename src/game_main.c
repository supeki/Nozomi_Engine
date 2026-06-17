// Nozomi Engine
// game_main.c

#include "i_event.h"
#include "i_sound.h"
#include "i_system.h"
#include "i_video.h"

#include "game_defs.h"
#include "game_gfx.h"
#include "game_input.h"
#include "game_main.h"
#include "game_object.h"
#include "game_player.h"
#include "game_sound.h"
#include "game_video.h"

bool game_quit = false;

// Game startup / main function.
void gameMain(void)
{
	I_printf("Initializing video...\n");
	V_Init();
	
	I_printf("Loading palette...\n");
	V_LoadPalette();
	
	I_printf("Starting graphics backend...\n");
	I_StartupGraphics();
	
	I_printf("Starting sound backend...\n");
	I_StartupSound();
	
	I_printf("Setting default controls...\n");
	G_DefaultControls();
	
	GFX_InitGFX();
	OBJ_InitObjects();
	
	P_CreatePlayer(rand()%(BASEVIDWIDTH-24) << SUBPIXEL_SHIFT, rand()%(BASEVIDHEIGHT-32) << SUBPIXEL_SHIFT, rand()%4);
	S_PlayMusic(mus_test, true);
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
			
			gameDisplay(); // Run all draw loops before pushing to the screen!
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
		for (int i = 0; i < num_players; i++)
			P_PlayerLogic(players[i]);
		OBJ_RunObjects();
	}
}

void gameDisplay(void)
{
	V_DrawText("meow\nmeower\n             meowing so hard\n meow meow", 0, 0, 0);
}
