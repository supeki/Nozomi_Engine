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

void BMPGFX(const char *filename)
{
	SDL_Surface *bmp = SDL_ConvertSurfaceFormat(SDL_LoadBMP(filename), SDL_PIXELFORMAT_RGB565, 0);
	
	if (bmp == NULL)
		return;

	FILE *file = fopen(va("%s.gfx", filename), "w+b");
	
	uint32_t size = bmp->w * bmp->h - 1;
	uint16_t width = bmp->w - 1;
	uint8_t offx = 0;
	uint8_t offy = 0;
	
	fwrite(&size, sizeof(uint32_t), 1, file);
	fwrite(&width, sizeof(uint16_t), 1, file);
	fwrite(&offx, sizeof(uint8_t), 1, file);
	fwrite(&offy, sizeof(uint8_t), 1, file);
	
	uint16_t* pixels = bmp->pixels;
	
	for (int i = 0; i < size; i++)
		for (int p = 0; p < 39; p++) {
			if (pixels[i] == palette[p]) {
				fwrite(&p, sizeof(uint8_t), 1, file);
			}
		}
			
	fclose(file);
}
