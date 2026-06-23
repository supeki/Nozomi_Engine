// Nozomi Engine
// game_defs.h

#ifndef GAME_DEFS_H
#define GAME_DEFS_H

#ifndef SATURN
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

// useful functions
char *va(const char *format, ...);
#else
#include <stddef.h>
#include <stdint.h>
#endif

#ifndef SATURN
#define nMalloc malloc
#endif


#define GAME_NAME "JADEFRACTURE"

#define FRAMERATE 30

// types
typedef int32_t subpixel_t;
#define SUBPIXEL_SHIFT 16
#define TO_PIXELS(i) (i/(1<<SUBPIXEL_SHIFT))
#define TO_SUBPIXELS(i) (i*(1<<SUBPIXEL_SHIFT))	

#endif
