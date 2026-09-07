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
extern float scale; // for scaling mouse as well
extern int mouse_offx, mouse_offy;

void I_UpdateWindow(SDL_Event event);
void I_ChangeWindowSize(int width, int height);

#endif
