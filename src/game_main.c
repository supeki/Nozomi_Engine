// Nozomi Engine
// game_main.c

#include "i_event.h"
#include "i_net.h"
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
	V_LoadPalette("data/palette.mpl", palette);
	
	I_printf("Starting graphics backend...\n");
	I_StartupGraphics();
	
	I_printf("Starting sound backend...\n");
	I_StartupSound();

	I_printf("Starting networking backend...\n");
	I_StartupNetwork();

	I_printf("Setting default controls...\n");
	G_DefaultControls();

	GFX_InitGFX();
	OBJ_InitObjects();

	//W_CreateTilesetFromFile("tech_demo", "tech_demo", 8);
	W_StartTilesetEdit(NULL, NULL);

	I_PlayMusic(mus_demo, true);
	//D_StartDialogue(0);

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
		
		// dumbass hack for SDL and WINCE ports
		#if defined(SDL) || defined(WINCE)
		{
			int j, c;
			for (j = 0; j < MAX_PLAYERS; j++) {
				for (c = CON_UP; c < NUMCONTROLS; c++)
					if (gamecontrols[j][c] > 0)
						gamecontrols[j][c]++;
				for (c = MOUSE_LBUTTON; c < NUMMOUSECONTROLS; c++)
					if (mousecontrols[j][c] > 0)
						mousecontrols[j][c]++;
			}
		}
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

	OBJ_FreeObjects();
	W_Free();
	I_ShutdownNetwork();
	I_ShutdownSound();
	I_ShutdownGraphics();
	V_Free();
}

void gameRunStuff(uint32_t elapsed)
{
	if (elapsed > 4)
		elapsed = 1;
	
	while (elapsed--)
	{	
		int i;

		if (in_diag) {
			D_UpdateDialogue();
			return;
		}

		if (font_edit)
			FNT_FontEditUpdate();

		if (tileset_edit)
			W_UpdateTilesetEdit();
	}
}

void gameDisplay(void)
{
	if (font_edit)
		FNT_FontEditDraw();

	if (tileset_edit)
		W_DrawTilesetEdit();

	if (in_diag)
		D_DrawDialogue();
}
