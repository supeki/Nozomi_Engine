// Nozomi Engine
// X11 backend
// x11_main.h

#ifndef X11_MAIN_H
#define X11_MAIN_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/time.h>
#include <unistd.h>

extern Display *Disp;
extern Window RootWnd, MainWnd;
extern Atom DeleteMsg;

#endif
