// Nozomi Engine
// game_main.c

#include "i_event.h"
#include "i_sound.h"
#include "i_system.h"
#include "i_video.h"

#include "game_defs.h"
#include "game_gfx.h"
#include "game_font.h"
#include "game_input.h"
#include "game_main.h"
#include "game_object.h"
#include "game_player.h"
#include "game_sound.h"
#include "game_video.h"

bool game_quit = false;
bitmap_gfx_t test;
uint32_t game_tick;

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
	
	//I_PlayMusic(mus_test, true);
	
	test = BMPGFX_LoadBitmap("data/test_gfx/test_kitsune.bmp");
	
	//FNT_StartFontEdit();
}

// The main game loop.
void gameLoop(void)
{
	uint32_t elapsed_tick, old_tick = I_GetTicks();
	uint32_t render_tick = 0;
	
	while (!game_quit) {
		game_tick = I_GetTicks();
		elapsed_tick = game_tick - old_tick;
		old_tick = game_tick;
		
		if (elapsed_tick == 0)
		{
			I_Sleep(1);
			continue;
		}
		
		// dumbass hack for SDL
		#if defined(SDL)
		for (int j = 0; j < MAX_PLAYERS; j++)
			for (int c = CON_UP; c < NUMCONTROLS; c++)
				if (gamecontrols[j][c] > 0)
					gamecontrols[j][c]++;
		#endif
		
		I_PollEvents();

		if (game_quit)
			break;
		
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
		if (font_edit)
			FNT_FontEditUpdate();
	}
}

void gameDisplay(void)
{
	if (font_edit)
		FNT_FontEditDraw();
	
	V_DrawBitmap(test, 0, 0, 0);
	V_DrawText("OMG LOOK AT THE KITSUNE :OOO", 0, 0, 0);
	V_DrawText("ooooo wavyyyyyyyy :D\nmeowwww", 0, 16, V_WAVYTEXT);
	V_DrawText("omg look at the second kitsune :O", 60, 80, V_WAVY);
	V_DrawBitmap(test, VID_WIDTH-120, VID_HEIGHT-100, V_WAVY|V_SMALL);
}
