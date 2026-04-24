// JADEFRACTURE
// game_misc.c

// Misc functions, variables, etc.
// Some useful, some maybe not. Nozomi

#include "game_defs.h"

char *va(const char *format, ...)
{
	va_list ap;
	va_list ap2;	

	va_start(ap, format);
	va_copy(ap2, ap);
	int len = vsnprintf(NULL, 0, format, ap2);
	va_end(ap2);
	
	if (len < 0) return NULL;
	
	char *buffer = malloc(len + 1);
	if (!buffer) return NULL;
	
	vsnprintf(buffer, len + 1, format, ap);
	
	va_end(ap);
	return buffer;
}
