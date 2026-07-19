// Nozomi Engine
// SDL2 backend
// sdl_video.c

#include <SDL2/SDL.h>

#include "sdl_main.h"

#include "../../i_system.h"
#include "../../i_video.h"
#include "../../game_video.h"
#include "../../game_defs.h"

SDL_Window *sdlWnd;
SDL_Surface *sdlSurf;
SDL_Renderer *wndRend;
SDL_Texture *sdlTex;

uint16_t* pixels;
uint32_t win_width = VID_WIDTH, win_height = VID_HEIGHT;
float scale;

void I_StartupGraphics(void)
{		
	pixels = malloc(VID_WIDTH * VID_HEIGHT * sizeof(uint16_t));
	scale = 1.0f;

	sdlWnd = SDL_CreateWindow(
		GAME_NAME, 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		VID_WIDTH,
		VID_HEIGHT,
		SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE
	);
	
	if (!sdlWnd) 
		I_Error("Failed to create window!\n");
	
	//SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
	wndRend = SDL_CreateRenderer(sdlWnd, -1, SDL_RENDERER_ACCELERATED);
	
	if (!wndRend) 
		I_Error("Failed to create renderer!\n");
	
	sdlTex = SDL_CreateTexture(
		wndRend, 
		SDL_PIXELFORMAT_RGB565, 
		SDL_TEXTUREACCESS_STREAMING, 
		VID_WIDTH, 
		VID_HEIGHT
	);
	
	if (!sdlTex) 
		I_Error("Failed to create texture!\n");
}

void I_UpdateWindow(SDL_Event event)
{	
	switch (event.window.event)
	{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			SDL_GetWindowSize(sdlWnd, &win_width, &win_height);
			
			scale = (float)win_height / (float)VID_HEIGHT;
			float xscale = (float)win_width / (float)VID_WIDTH;
			
			if (xscale < scale)
				scale = xscale;
			
			SDL_RenderPresent(wndRend);
			break;
	}
}

void I_PushGraphics(void)
{
	int width = (int)(scale*(float)VID_WIDTH);
	int height = (int)(scale*(float)VID_HEIGHT);
	SDL_Rect dest_rect[4] = {(win_width/2) - (width/2), (win_height/2) - (height/2), width, height};

	memcpy(pixels, vid.buffer, VID_WIDTH * VID_HEIGHT * sizeof(uint16_t));

	SDL_RenderClear(wndRend);
	SDL_UpdateTexture(sdlTex, NULL, pixels, VID_WIDTH * sizeof(uint16_t));
	SDL_RenderCopy(wndRend, sdlTex, NULL, dest_rect);
	SDL_RenderPresent(wndRend);
}
