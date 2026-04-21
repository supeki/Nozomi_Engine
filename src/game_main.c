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
gfx_t test;
object_t maril;

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
	
	test = GFX_LoadGFX("marilyn.bmp.gfx");
	memset(&maril, 0, sizeof(object_t));
}

// The main game loop.
void gameLoop(void)
{
	int game_tick, elapsed_tick;
	int old_tick = I_GetTicks();
	int render_tick = 0;
	
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

static int walk_table[4] = {0, 1, 0, 2};

void gameRunStuff(uint32_t elapsed)
{
	if (elapsed > 4)
		elapsed = 1;
	
	while (elapsed--)
	{	
		// o'tayyy doing this for save
		int8_t xmove = G_ControlDown(CON_RIGHT, false) - G_ControlDown(CON_LEFT, false);
		int8_t ymove = G_ControlDown(CON_DOWN, false) - G_ControlDown(CON_UP, false);
			
		if (G_ControlDown(CON_LEFT, false) && !G_ControlDown(CON_RIGHT, false)) {
			if (!(G_ControlDown(CON_UP, false) || G_ControlDown(CON_DOWN, false)) || maril.dir == 3)
			maril.dir = 1;
		}
		
		if (G_ControlDown(CON_RIGHT, false) && !G_ControlDown(CON_LEFT, false)) {
			if (!(G_ControlDown(CON_UP, false) || G_ControlDown(CON_DOWN, false)) || maril.dir == 1)
			maril.dir = 3;
		}
		
		maril.momx = (xmove*(2<<SUBPIXEL_SHIFT));
		
		if (G_ControlDown(CON_UP, false) && !G_ControlDown(CON_DOWN, false)) {
			if (!(G_ControlDown(CON_LEFT, false) || G_ControlDown(CON_RIGHT, false)) || maril.dir == 0)
			maril.dir = 2;
		}
		
		if (G_ControlDown(CON_DOWN, false) && !G_ControlDown(CON_UP, false)) {
			if (!(G_ControlDown(CON_LEFT, false) || G_ControlDown(CON_RIGHT, false)) || maril.dir == 2)
			maril.dir = 0;
		}
		
		maril.momy = (ymove*(2<<SUBPIXEL_SHIFT));
		
		if (abs(xmove) && abs(ymove)) {
			maril.momx = (subpixel_t)((float)maril.momx * (1.0f / sqrt(2.0f)));
			maril.momy = (subpixel_t)((float)maril.momy * (1.0f / sqrt(2.0f)));
		}
			
		if (abs(maril.momx) + abs(maril.momy) > 0) {
			maril.x += maril.momx;
			maril.y += maril.momy;
			
			maril.anim_timer++;
			V_DrawCroppedSprite(test, TO_PIXELS(maril.x), TO_PIXELS(maril.y), walk_table[(maril.anim_timer/6) % 4]*24, maril.dir*32, 24, 32);
		} else {
			maril.anim_timer = 0;
			V_DrawCroppedSprite(test, TO_PIXELS(maril.x), TO_PIXELS(maril.y), 0, maril.dir*32, 24, 32);
		}
	}
}