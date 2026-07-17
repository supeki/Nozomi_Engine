// Nozomi Engine
// SDL2 backend
// sdl_input.h

#ifndef SDL_INPUT_H
#define SDL_INPUT_H

#include <SDL2/SDL.h>

void I_UpdateControls(SDL_Event event);
void I_UpdateControllerAxis(void);

#endif
