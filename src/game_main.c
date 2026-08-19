// Nozomi Engine
// game_main.c

#include "i_event.h"
#include "i_sound.h"
#include "i_system.h"
#include "i_video.h"

#include "game_defs.h"
#include "game_dialogue.h"
#include "game_gfx.h"
#include "game_font.h"
#include "game_input.h"
#include "game_main.h"
#include "game_object.h"
#include "game_player.h"
#include "game_sound.h"
#include "game_video.h"
#include "game_world.h"

bool game_quit = false;
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
	P_CreatePlayer(128*PU - 12*PU, 536*PU, 2);
	OBJ_CreateObject(128*PU, 64*PU, OBJ_MAN);
	I_PlayMusic(mus_man, true);

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
			
			gameDisplay(); // Run all draw loops before pushing to the screen.
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
		int i;

		if (font_edit)
			FNT_FontEditUpdate();
		
		if (in_diag) {
			D_UpdateDialogue();
			return;
		}
			
		for (i = 0; i < num_players; i++)
			P_PlayerLogic(players[i]);
		
		OBJ_RunObjects();
	}
}

void gameDisplay(void)
{
	int i, treeoff_1, treeoff_2;

	if (font_edit)
		FNT_FontEditDraw();
	
	for (i = 0; i < 576; i++)
		V_DrawCroppedBitmap(gfx_tiles, (i%16)*16, (i/16)*16 - camera.y, demo_tiles[i]*16, 0, 16, 16, 0);
	
	OBJ_DrawObjectLayer(0);
	
	treeoff_1 = abs((I_GetTicks()/30) % 8 - 4) + 4;
	treeoff_2 = -abs((I_GetTicks()/15) % 4 - 2) + 2;
	
	V_DrawBitmap(gfx_tree2, 128 - gfx_tree.width/2 + treeoff_1, 96 - gfx_tree.height - camera.y + -treeoff_2, 0);
	V_DrawBitmap(gfx_tree3, 128 - gfx_tree.width/2 + -treeoff_1, 96 - gfx_tree.height - camera.y + treeoff_2, 0);
	
	if (in_diag)
		D_DrawDialogue();
}

uint8_t demo_tiles[576] = {
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	00,00,00,00,00,00,01,01,01,01,00,00,00,00,00,00,
	00,00,00,00,00,01,01,01,01,01,01,00,00,00,00,00,
	00,00,00,00,00,01,01,01,01,01,01,00,00,00,00,00,
	00,00,00,00,00,01,01,01,01,01,01,00,00,00,00,00,
	00,00,00,00,00,00,01,01,01,01,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,01,01,00,00,00,00,00,00,00,
};