// JADEFRACTURE
// SDL2 backend
// sdl_main.h

#include <SDL.h>

#ifndef SDL_MAIN_H
#define SDL_MAIN_H

extern SDL_Window *sdlWnd;
extern SDL_Surface *sdlSurf;
extern SDL_Surface *wndSurf;

void BMPGFX(const char *filename);

#endif