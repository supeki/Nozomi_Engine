// Nozomi Engine
// Xbox/nxdk backend
// xbox_video.c

#include "xbox_main.h"

#include "../../i_system.h"
#include "../../i_video.h"
#include "../../game_video.h"
#include "../../game_defs.h"

SDL_Window *sdlWnd;
SDL_Surface *sdlSurf;
SDL_Renderer *wndRend;
SDL_Texture *sdlTex;

uint16_t* pixels;
SDL_Texture *borderTex;
uint32_t win_width = 640, win_height = 480;
float scale;

void I_StartupGraphics(void)
{		
    XVideoSetMode(win_width, win_height, 32, REFRESH_DEFAULT);
    SDL_VideoInit(NULL);

    SDL_GetCurrentVideoDriver();

    pixels = malloc(VID_WIDTH * VID_HEIGHT * sizeof(uint16_t));
    scale = (float)win_height / (float)VID_HEIGHT;
	float xscale = (float)win_width / (float)VID_WIDTH;
	
	if (xscale < scale)
		scale = xscale;

	sdlWnd = SDL_CreateWindow(
		GAME_NAME, 
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		win_width,
		win_height,
		SDL_WINDOW_SHOWN
	);
	
	if (!sdlWnd) 
		I_Error("Failed to create window!\n");
	
	wndRend = SDL_CreateRenderer(sdlWnd, -1, 0);

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

	//SDL_Surface *borderSurf = SDL_LoadBMP("data/border.bmp");
	//borderTex = SDL_CreateTextureFromSurface(wndRend, borderSurf);
	//SDL_FreeSurface(borderSurf);
}

void I_ShutdownGraphics(void)
{	
    SDL_DestroyTexture(sdlTex);
	SDL_DestroyRenderer(wndRend);
	SDL_DestroyWindow(sdlWnd);
	free(pixels);
}

void I_PushGraphics(void)
{
	int width = (int)(scale*(float)VID_WIDTH);
	int height = (int)(scale*(float)VID_HEIGHT);
	SDL_Rect dest_rect[4] = {(win_width/2) - (width/2), (win_height/2) - (height/2), width, height};

	mouse_offx = (win_width/2) - (width/2); 
	mouse_offy = (win_height/2) - (height/2);

	memcpy(pixels, vid.buffer, VID_WIDTH * VID_HEIGHT * sizeof(uint16_t));

	SDL_RenderClear(wndRend);
	SDL_UpdateTexture(sdlTex, NULL, pixels, VID_WIDTH * sizeof(uint16_t));

	//SDL_Rect xbox_border[4] = {0, 0, win_width, win_height};
	//SDL_RenderCopy(wndRend, borderTex, NULL, xbox_border);

	SDL_RenderCopy(wndRend, sdlTex, NULL, dest_rect);
	SDL_RenderPresent(wndRend);
}
