// Nozomi Engine
// X11 backend
// x11_video.c

#include "x11_main.h"

#include "../../i_system.h"
#include "../../i_video.h"

#include "../../game_video.h"
#include "../../game_defs.h"

// meow meow, all defines hopefully
Display* Disp;
Window RootWnd, MainWnd;
Atom DeleteMsg;

// these ones should be local to this file
Visual *Vis;
GC gc;
XImage *xImage;
int Scr, ScrDepth;
uint8_t *FrameBuffer;

void I_StartupGraphics(void)
{
	XWindowAttributes RootAttrs;
	XSetWindowAttributes WndAttrs;
	unsigned long WndMask = CWBackPixel | CWOverrideRedirect | CWEventMask;

	Disp = XOpenDisplay(0);
	Scr = DefaultScreen(Disp);
	Vis = DefaultVisual(Disp, Scr);
	ScrDepth = DefaultDepth(Disp, Scr);

	// Make our window mrrp
	RootWnd = XDefaultRootWindow(Disp);
	XGetWindowAttributes(Disp, RootWnd, &RootAttrs);

	WndAttrs.background_pixel = BlackPixel(Disp, Scr);
	WndAttrs.override_redirect = True;
	WndAttrs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask;

	MainWnd = XCreateWindow(Disp, RootWnd, 0, 0, RootAttrs.width, RootAttrs.height, 2, ScrDepth, InputOutput, Vis, WndMask, &WndAttrs);
	DeleteMsg = XInternAtom(Disp, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(Disp, MainWnd, &DeleteMsg, 1);
	XMapWindow(Disp, MainWnd);
	XRaiseWindow(Disp, MainWnd);
	XSetInputFocus(Disp, MainWnd, RevertToParent, CurrentTime);
	XFlush(Disp);

	// Setup framebuffer and image,,
	gc = XCreateGC(Disp, MainWnd, 0, NULL);
	FrameBuffer = malloc(VID_WIDTH * VID_HEIGHT * 4); // 4 bits per pixel blegh
	xImage = XCreateImage(Disp, Vis, ScrDepth, ZPixmap, 0, (char *)FrameBuffer, VID_WIDTH, VID_HEIGHT, 32, 0);
}

void I_ShutdownGraphics(void)
{
	XDestroyWindow(Disp, MainWnd);
	XDestroyWindow(Disp, RootWnd);
	XCloseDisplay(Disp);
}

void I_PushGraphics(void)
{
	uint32_t *dest = (uint32_t *)xImage->data;

	// convert our framebuffer to ARGB8888 for image,,
	for (uint32_t i = 0; i < VID_WIDTH*VID_HEIGHT; i++) {
		uint16_t src = vid.buffer[i];
		uint8_t r, g, b;

		r = ((src >> 11) & 0x1F) << 3;
		g = ((src >> 5) & 0x3F) << 2;
		b = (src & 0x1F) << 3;

		dest[i] = (r << 16) | (g << 8) | b;
	}

	// put image on window and flush to screen maow
	XPutImage(Disp, MainWnd, gc, xImage, 0, 0, 0, 0, VID_WIDTH, VID_HEIGHT);
	XFlush(Disp);
}
