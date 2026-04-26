// JADEFRACTURE
// SDL2 backend
// sdl_video.c

#include <SDL2/SDL.h>

#include "sdl_main.h"

#include "../../i_system.h"
#include "../../i_video.h"
#include "../../game_video.h"

SDL_Window *sdlWnd;
SDL_Surface *sdlSurf;
SDL_Renderer *wndRend;
SDL_Texture *sdlTex;

uint16_t* pixels;
float scale;

void I_StartupGraphics(void)
{	
	#if defined(PSP)
	vid.width = 480;
	vid.height = 272;
	#endif
	
	pixels = malloc(BASEVIDWIDTH * BASEVIDHEIGHT * sizeof(uint16_t));
	scale = (float)vid.height / (float)BASEVIDHEIGHT;

	sdlWnd = SDL_CreateWindow(
		"JADEFRACTURE", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		vid.width,
		vid.height,
		SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE
	);
	
	if (!sdlWnd) 
		I_Error("Failed to create window!\n");
	
	wndRend = SDL_CreateRenderer(sdlWnd, -1, SDL_RENDERER_ACCELERATED);
	
	if (!wndRend) 
		I_Error("Failed to create renderer!\n");
	
	sdlTex = SDL_CreateTexture(
		wndRend, 
		SDL_PIXELFORMAT_RGB565, 
		SDL_TEXTUREACCESS_STREAMING, 
		BASEVIDWIDTH, 
		BASEVIDHEIGHT
	);
	
	if (!sdlTex) 
		I_Error("Failed to create texture!\n");
}

void I_UpdateWindow(SDL_Event event)
{	
	switch (event.window.event)
	{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			SDL_SetWindowSize(sdlWnd, event.window.data1, event.window.data2);
			SDL_GetWindowSize(sdlWnd, &vid.width, &vid.height);
			
			scale = (float)vid.height / (float)BASEVIDHEIGHT;
			float xscale = (float)vid.width / (float)BASEVIDWIDTH;
			if (xscale < scale)
				scale = xscale;
			
			SDL_RenderPresent(wndRend);
			break;
	}
}

void I_PushGraphics(void)
{
	int width = (int)(scale*(float)BASEVIDWIDTH);
	int height = (int)(scale*(float)BASEVIDHEIGHT);
	SDL_Rect dest_rect[4] = {(vid.width/2) - (width/2), (vid.height/2) - (height/2), width, height};

	for (int i = 0; i < BASEVIDWIDTH * BASEVIDHEIGHT; i++)
	{
		pixels[i] = palette[vid.buffer[i]];
	}
	
	SDL_RenderClear(wndRend);
	SDL_UpdateTexture(sdlTex, NULL, pixels, BASEVIDWIDTH * sizeof(uint16_t));
	SDL_RenderCopy(wndRend, sdlTex, NULL, dest_rect);
	SDL_RenderPresent(wndRend);
}

void BMPGFX(const char *filename)
{
	FILE *file = fopen(va("%s.gfx", filename), "w+b");
	SDL_Surface *bmp = SDL_ConvertSurfaceFormat(SDL_LoadBMP(filename), SDL_PIXELFORMAT_RGB565, 0);
	uint32_t size = bmp->w * bmp->h - 1;
	uint8_t width = bmp->w - 1;
	uint8_t offx = 0;
	uint8_t offy = 0;
	
	fwrite(&size, sizeof(uint16_t), 1, file);
	fwrite(&width, sizeof(uint8_t), 1, file);
	fwrite(&offx, sizeof(uint8_t), 1, file);
	fwrite(&offy, sizeof(uint8_t), 1, file);
	
	uint16_t* pixels = bmp->pixels;
	
	for (int i = 0; i < size; i++)
		for (int p = 0; p < 39; p++) {
			if (pixels[i] == palette[p]) {
				uint8_t h = (p << 1);
			
				if (pixels[i] == 0)
					h++;
				
				fwrite(&h, sizeof(uint8_t), 1, file);
			}
		}
			
	fclose(file);
}
