// Nozomi Engine
// DOS/DJGPP backend
// dos_video.c

#include "dos_main.h"
#include "../../i_video.h"

#include "../../game_defs.h"
#include "../../game_video.h"

static BITMAP* vid_bitmap;

void I_StartupGraphics(void)
{
	set_color_depth(16);
	set_gfx_mode(GFX_AUTODETECT, 320*(VID_WIDTH/256), 200*(VID_HEIGHT/192), 0, 0);

    mouse_offx = (320*(VID_WIDTH/256) - VID_WIDTH) / 2;
    mouse_offy = (200*(VID_HEIGHT/192) - VID_HEIGHT) / 2;

    vid_bitmap = create_bitmap(VID_WIDTH, VID_HEIGHT);
}

void I_ShutdownGraphics(void)
{
    if (vid_bitmap != NULL) { 
        destroy_bitmap(vid_bitmap); 
        vid_bitmap = NULL; 
    }

	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
}

void I_PushGraphics(void)
{
	uint16_t* pixels = (uint16_t*)vid_bitmap->dat;
	uint16_t x, y;

	for (int y = 0; y < VID_HEIGHT; y++) {
        uint16_t* dest_line = (uint16_t*)vid_bitmap->line[y];
        uint16_t* src_line = (uint16_t*)&vid.buffer[y * VID_WIDTH];
        
        memcpy(dest_line, src_line, VID_WIDTH * sizeof(uint16_t));
    }
    
    show_mouse(NULL);

	stretch_blit(
		vid_bitmap,
		screen,
		0, 0,
		VID_WIDTH, VID_HEIGHT,
		(320*(VID_WIDTH/256) - VID_WIDTH) / 2, (200*(VID_HEIGHT/192) - VID_HEIGHT) / 2,
		VID_WIDTH, VID_HEIGHT
	);

    show_mouse(screen);
}