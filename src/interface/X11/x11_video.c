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
//  width       height       screen scale
int disp_width, disp_height, iscale;
//  offsets
int x_offset, y_offset;

void I_StartupGraphics(void)
{
	XWindowAttributes RootAttrs;
	XSetWindowAttributes WndAttrs;
	unsigned long WndMask = CWBackPixel | CWOverrideRedirect | CWEventMask;
	int hscale, vscale;

	Disp = XOpenDisplay(0);
	Scr = DefaultScreen(Disp);
	Vis = DefaultVisual(Disp, Scr);
	ScrDepth = DefaultDepth(Disp, Scr);

	// Make our window mrrp
	RootWnd = XDefaultRootWindow(Disp);
	XGetWindowAttributes(Disp, RootWnd, &RootAttrs);
	disp_width = RootAttrs.width;
	disp_height = RootAttrs.height;
	hscale = disp_width / VID_WIDTH;
	vscale = disp_height / VID_HEIGHT;
	if (hscale < vscale)
		iscale = hscale;
	else
		iscale = vscale;
	if (iscale < 1)
		iscale = 1;

	x_offset = (disp_width - (VID_WIDTH*iscale))/2;
	y_offset = (disp_height - (VID_HEIGHT*iscale))/2;

	WndAttrs.background_pixel = BlackPixel(Disp, Scr);
	WndAttrs.override_redirect = True;
	WndAttrs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask;

	MainWnd = XCreateWindow(Disp, RootWnd, 0, 0, disp_width, disp_height, 2, ScrDepth, InputOutput, Vis, WndMask, &WndAttrs);
	DeleteMsg = XInternAtom(Disp, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(Disp, MainWnd, &DeleteMsg, 1);
	XMapWindow(Disp, MainWnd);
	XRaiseWindow(Disp, MainWnd);
	XSetInputFocus(Disp, MainWnd, RevertToParent, CurrentTime);
	XFlush(Disp);

	// Setup framebuffer and image,,
	gc = XCreateGC(Disp, MainWnd, 0, NULL);
	FrameBuffer = malloc((VID_WIDTH*iscale) * (VID_HEIGHT*iscale) * 4); // 4 bits per pixel blegh
	xImage = XCreateImage(Disp, Vis, ScrDepth, ZPixmap, 0, (char *)FrameBuffer, VID_WIDTH*iscale, VID_HEIGHT*iscale, 32, 0);
}

void I_ShutdownGraphics(void)
{
	XDestroyWindow(Disp, MainWnd);
	XDestroyWindow(Disp, RootWnd);
	XCloseDisplay(Disp);
}

// very similar to V_DrawCroppedScaled in a sense
void I_PushGraphics(void)
{
	int zx, zy, px, py, dest_x, dest_y;
	uint32_t *dst = (uint32_t *)xImage->data;

	// convert our framebuffer to ARGB8888 for image,, (and scale it!!)
	for (zy = 0; zy < VID_HEIGHT; zy++) {
		dest_y = zy * iscale;

		for (zx = 0; zx < VID_WIDTH; zx++) {
			uint8_t r, g, b;
			uint16_t src = vid.buffer[zx + zy*VID_WIDTH];

			dest_x = zx * iscale;

			r = ((src >> 11) & 0x1F) << 3;
			g = ((src >> 5) & 0x3F) << 2;
			b = (src & 0x1F) << 3;

			for (py = dest_y; py < dest_y + iscale; py++)
				for (px = dest_x; px < dest_x + iscale; px++)
					dst[px + py * (VID_WIDTH*iscale)] = (r << 16) | (g << 8) | b;
		}
	}

	// put image on window and flush to screen maow
	XPutImage(Disp, MainWnd, gc, xImage, 0, 0, x_offset, y_offset, VID_WIDTH*iscale, VID_HEIGHT*iscale);
	XFlush(Disp);
}
