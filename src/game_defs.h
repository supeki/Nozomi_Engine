// JADEFRACTURE
// game_defs.h

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

#ifndef GAME_DEFS_H
#define GAME_DEFS_H

#define FRAMERATE 30

// useful functions
char *va(const char *format, ...);

#endif