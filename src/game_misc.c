// Nozomi Engine
// game_misc.c

// Misc functions, variables, etc.
// Some useful, some maybe not. Nozomi

#include "game_defs.h"

#if defined(GBC)
#define VA_BUF_SIZE 32 
#else
#define VA_BUF_SIZE 256
#endif

char *va(const char *fmt, ...) {
    static char buf[4][VA_BUF_SIZE];
    static int idx = 0;
    va_list ap;

    idx = (idx + 1) & 3;
    va_start(ap, fmt);

    // stupid implementation for GBC, killing myself
    #if defined(GBC)
    char *dest = buf[idx];
    char *p = dest;
    char *end = dest + VA_BUF_SIZE - 1;

    while (*fmt && p < end) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'c': 
                {
                    *p++ = (char)va_arg(ap, int);
                    break;
                }

                case 'd': 
                {
                    int val = va_arg(ap, int);
                    char tmp[7];
                    itoa(val, tmp, 10);
                    
                    char *t = tmp;
                    while (*t && p < end) {
                        *p++ = *t++;
                    }
                    break;
                }

                case 'u': 
                {
                    uint32_t val = va_arg(ap, uint32_t);
                    char tmp[6];
                    uitoa(val, tmp, 10);
                    
                    char *t = tmp;
                    while (*t && p < end) {
                        *p++ = *t++;
                    }
                    break;
                }

                case 's': 
                {
                    char *str = va_arg(ap, char*);
                    while (*str && p < end) {
                        *p++ = *str++;
                    }
                    break;
                }

                case '%': 
                {
                    *p++ = '%';
                    break;
                }

                default: 
                {
                    *p++ = *fmt;
                    break;
                }
            }
        } else {
            *p++ = *fmt;
        }
        fmt++;
    }

    *p = '\0';
    #else
    vsnprintf(buf[idx], 256, fmt, ap);
    #endif

    va_end(ap);

    return buf[idx];
}
