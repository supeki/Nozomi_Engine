// Nozomi Engine
// Xbox/nxdk backend
// xbox_main.h

#ifndef XBOX_MAIN_H
#define XBOX_MAIN_H

#include <hal/debug.h>
#include <hal/xbox.h>
#include <hal/video.h>
#include <windows.h>
#include <SDL.h>

#include "../../game_defs.h"
#include "../../game_video.h"

extern SDL_Window *sdlWnd;
extern SDL_Surface *sdlSurf;
extern SDL_Surface *wndSurf;
extern float scale; // for scaling mouse as well
extern int mouse_offx, mouse_offy;

#endif
