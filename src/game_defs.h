// Nozomi Engine
// game_defs.h

#ifndef GAME_DEFS_H
#define GAME_DEFS_H

#ifdef WIN32
#include <windows.h>
#endif

#if defined(PSP)
#include <unistd.h>
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define GAME_NAME "JADEFRACTURE"

#define FRAMERATE 30
#define MAX_PLAYERS 4

#define PLAYER_ONE 0
#define PLAYER_TWO 1
#define PLAYER_THREE 2
#define PLAYER_FOUR 3

// useful functions
char *va(const char *format, ...);

// types
typedef int32_t subpixel_t;
#define SUBPIXEL_SHIFT 16
#define TO_PIXELS(i) (i/(1<<SUBPIXEL_SHIFT))
#define TO_SUBPIXELS(i) (i*(1<<SUBPIXEL_SHIFT))

#endif
