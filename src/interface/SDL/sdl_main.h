// Nozomi Engine
// SDL2 backend
// sdl_main.h

#ifndef SDL_MAIN_H
#define SDL_MAIN_H

#include <SDL2/SDL.h>
#include "../../game_defs.h"

extern SDL_Window *sdlWnd;
extern SDL_Surface *sdlSurf;
extern SDL_Surface *wndSurf;

void I_UpdateWindow(SDL_Event event);
void BMPGFX(const char *filename);

#endif
