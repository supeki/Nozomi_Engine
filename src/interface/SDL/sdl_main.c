// JADEFRACTURE
// SDL2 backend
// sdl_main.c

#include <SDL.h>

#include "sdl_main.h"

#include "../../i_event.h"
#include "../../i_system.h"
#include "../../i_video.h"

#include "../../game_main.h"
#include "../../game_video.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
	
	printf("Initializing SDL!\n");
	
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0)
	{
		printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
	}
	
	printf("Successfully initialized SDL!\n");
	
	/*
	// Load a test bitmap!
	{
		int i;
		SDL_Surface *loading = SDL_ConvertSurfaceFormat(SDL_LoadBMP("test.bmp"), SDL_PIXELFORMAT_RGB332, 0);
		byte *pixels = loading->pixels;
		
		for (i = 0; i < vid.width * vid.height; i++)
		{
			vid.buffer[i] = pixels[i];
		}
		
		SDL_FreeSurface(loading);
	}
	*/
	
	gameMain();
	gameLoop();
	
	printf("Quitting SDL!\n");
	
	SDL_Quit();
	printf("Quitting JADEFRACTURE...\n");
	
	exit(0);
}