// JADEFRACTURE
// game_defs.h

#ifndef GAME_DEFS_H
#define GAME_DEFS_H

#ifdef WIN32
#include <windows.h>
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define FRAMERATE 30

// useful functions
char *va(const char *format, ...);

// types
typedef int32_t subpixel_t;
#define SUBPIXEL_SHIFT 16
#define TO_PIXELS(i) (i/(1<<SUBPIXEL_SHIFT))

#endif
