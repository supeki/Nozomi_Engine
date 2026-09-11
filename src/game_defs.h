// Nozomi Engine
// game_defs.h

#ifndef GAME_DEFS_H
#define GAME_DEFS_H

#if defined (WINDOWS) || defined(WIN32) || defined(WINCE)
#include <windows.h>
#endif

#if defined(PSP)
#include <unistd.h>
#endif

#if defined(DOS)
#include <allegro.h>
#endif

#include <stdarg.h>
#if !defined(WINCE)
#include <stdbool.h>
#include <stdint.h>
#else
typedef signed __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef signed __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef signed __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef boolean bool;
#define false FALSE
#define true TRUE
#define vsnprintf _vsnprintf
#define snprintf _snprintf
#define rewind(f) (fseek((f), 0, SEEK_SET))
#define va_copy(dest, src) ((dest) = (src))
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef GAME_NAME
#define GAME_NAME "Nozomi Engine dev"
#endif

#define FRAMERATE 30

#if defined (WINCE)
#define MAX_PLAYERS 1
#else
#define MAX_PLAYERS 4
#endif

#define PLAYER_ONE 0
#define PLAYER_TWO 1
#define PLAYER_THREE 2
#define PLAYER_FOUR 3

// useful functions
char *va(const char *format, ...);

// types
#ifndef PI
#define PI 3.14159265358
#endif

#define PU 65536

extern uint8_t demo_tiles[576];

#endif
