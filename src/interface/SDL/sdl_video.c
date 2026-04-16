// JADEFRACTURE
// SDL2 backend
// sdl_video.c

#include <SDL.h>

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
		SDL_WINDOW_SHOWN|SDL_WINDOW_BORDERLESS
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
		SDL_PIXELFORMAT_RGB332
	);
	
	if (!sdlSurf)
	{
		printf("Failed to create surface!\n");
		exit(-1);
	}
}

void I_UpdateWindow(void)
{
	sdlWnd = SDL_CreateWindow(
		"JADEFRACTURE", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		vid.width,
		vid.height,
		SDL_WINDOW_SHOWN|SDL_WINDOW_BORDERLESS
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
		SDL_PIXELFORMAT_RGB332
	);
	
	if (!sdlSurf)
	{
		printf("Failed to create surface!\n");
		exit(-1);
	}
	
	vid.buffer = malloc(vid.width*vid.height);
	memset(vid.buffer, 0, vid.width*vid.height);
}

void I_PushGraphics(void)
{
	byte* pixels = sdlSurf->pixels;

	for (int i = 0; i < vid.width * vid.height; i++)
	{
		pixels[i] = vid.buffer[i];
	}
	
	SDL_BlitScaled(sdlSurf, NULL, wndSurf, NULL);
	SDL_UpdateWindowSurface(sdlWnd);
}