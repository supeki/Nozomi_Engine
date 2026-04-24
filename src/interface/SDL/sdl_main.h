// JADEFRACTURE
// SDL2 backend
// sdl_main.h

#ifndef SDL_MAIN_H
#define SDL_MAIN_H

#include <SDL.h>

extern SDL_Window *sdlWnd;
extern SDL_Surface *sdlSurf;
extern SDL_Surface *wndSurf;

void BMPGFX(const char *filename);

#endif
