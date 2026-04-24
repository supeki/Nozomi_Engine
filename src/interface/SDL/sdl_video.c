// JADEFRACTURE
// SDL2 backend
// sdl_video.c

#include <SDL.h>

#include "sdl_main.h"

#include "../../i_video.h"
#include "../../game_video.h"

SDL_Window *sdlWnd;
SDL_Surface *sdlSurf;
SDL_Surface *wndSurf;

void I_StartupGraphics(void)
{	
	sdlWnd = SDL_CreateWindow(
		"JADEFRACTURE", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		vid.width,
		vid.height,
		SDL_WINDOW_SHOWN
	);
	
	if (!sdlWnd) 
	{
		printf("Failed to create window!\n");
		exit(-1);
	}
	
	wndSurf = SDL_GetWindowSurface(sdlWnd);
	
	if (!wndSurf) 
	{
		printf("Failed to get window surface!\n");
		exit(-1);
	}
	
	sdlSurf = SDL_CreateRGBSurfaceWithFormat(
		0,
		vid.width,
		vid.height,
		8,
		SDL_PIXELFORMAT_RGB565
	);
	
	if (!sdlSurf)
	{
		printf("Failed to create surface!\n");
		exit(-1);
	}
}

void I_PushGraphics(void)
{
	uint16_t* pixels = sdlSurf->pixels;

	for (int i = 0; i < vid.width * vid.height; i++)
	{
		pixels[i] = palette[vid.buffer[i]];
	}
	
	SDL_BlitScaled(sdlSurf, NULL, wndSurf, NULL);
	SDL_UpdateWindowSurface(sdlWnd);
}

void BMPGFX(const char *filename)
{
	FILE *file = fopen(va("%s.gfx", filename), "w+b");
	SDL_Surface *bmp = SDL_ConvertSurfaceFormat(SDL_LoadBMP(filename), SDL_PIXELFORMAT_RGB565, 0);
	uint32_t size = bmp->w * bmp->h - 1;
	byte width = bmp->w - 1;
	byte offx = 0;
	byte offy = 0;
	
	fwrite(&size, sizeof(uint16_t), 1, file);
	fwrite(&width, sizeof(uint8_t), 1, file);
	fwrite(&offx, sizeof(uint8_t), 1, file);
	fwrite(&offy, sizeof(uint8_t), 1, file);
	
	uint16_t* pixels = bmp->pixels;
	
	for (int i = 0; i < size; i++)
		for (int p = 0; p < 39; p++) {
			if (pixels[i] == palette[p]) {
				byte h = (p << 1);
			
				if (pixels[i] == 0)
					h++;
				
				fwrite(&h, sizeof(uint8_t), 1, file);
			}
		}
			
	fclose(file);
}
