// Nozomi Engine
// game_world.c

#include "i_system.h"

#include "game_defs.h"
#include "game_input.h"
#include "game_object.h"
#include "game_world.h"

uint16_t world_width = 16, world_height = 16;
uint16_t tile_width = 8, tile_height = 8;
tile_t tiles;
tileattr_t tileattr;
gfx_t tile_gfx;
bool world_edit = false;
uint32_t *temp_world_data;

void W_InitTiles(void)
{
	tiles.prev = tiles.next = &tiles;
	tile_gfx = GFX_LoadGFX("data/tilesets/tech_demo_tiles.gfx");
}

/*
typedef struct tile_s
{
	struct tile_s *prev, next;
	
	uint32_t id;
	subpixel_t x, y; // position in the world
} tile_t;

typedef struct tileattr_s
{
	uint32_t num_tiles, tiles_per_row, *type;
	int8_t *layer;
} tileattr_t;
*/

static void W_LoadTileAttr(const char *filename)
{
	FILE *attr_fp = fopen(filename, "rb");
	
	// tile attributes
	fread(&tileattr.num_tiles, sizeof(uint32_t), 1, attr_fp);
	fread(&tileattr.tiles_per_row, sizeof(uint32_t), 1, attr_fp); // used for gfx when drawing
	
	tile_height = tile_width = (tile_gfx.width / tileattr.tiles_per_row); // needs to be divisible by 2 and square ratio?
	
	tileattr.type =  malloc(tileattr.num_tiles * sizeof(uint32_t));
	tileattr.layer = malloc(tileattr.num_tiles * sizeof(uint8_t));
	
	for (int i = 0; i < tileattr.num_tiles; i++)
		fread(&tileattr.type[i], sizeof(uint32_t), 1, attr_fp);
	for (int i = 0; i < tileattr.num_tiles; i++)
		fread(&tileattr.layer[i], sizeof(uint8_t), 1, attr_fp);
	
	fclose(attr_fp);
}

void W_LoadWorld(const char* filename)
{
	FILE *fp = fopen(filename, "rb"), *attr_fp;
	char tilefilename[33];
	
	// 2 bytes world width
	// 2 bytes world height
	fread(&world_width, sizeof(uint16_t), 1, fp);
	fread(&world_height, sizeof(uint16_t), 1, fp);
	
	// 32 bytes tile gfx filename (and tileattr)
	fread(&tilefilename, sizeof(char), 32, fp);
	
	tile_gfx = GFX_LoadGFX(va("data/tilesets/%s.gfx", tilefilename));
	W_LoadTileAttr(va("data/tilesets/%s.atr", tilefilename));
	
	if (world_edit)
		temp_world_data = malloc(world_width * world_height * sizeof(uint32_t));
	
	// 4 bytes object id
	// 2 bytes x
	// 2 bytes y
	for (uint32_t i = 0; i < (world_width * world_height); i++) {
		uint32_t id;

		fread(&id, sizeof(uint32_t), 1, fp);
		if (world_edit)
			temp_world_data[i] = id;
		else
			W_CreateTile((i % world_width)*tile_width, (i / world_width)*tile_height, id);
	}
	
	fclose(fp);
}

void W_HandleTiles(void)
{
	tile_t *tile = tiles.next;
	
	while (tile != &tiles)
	{		
		

		tile = tile->next;
	}
}

tile_t *W_CreateTile(subpixel_t x, subpixel_t y, uint8_t id)
{
	tile_t *tile;
	tile = malloc(sizeof(tile_t));
	memset(tile, 0, sizeof(tile_t));
	
	tile->id = id;
	tile->x = x;
	tile->y = y;
	
	// add this object to the list of objects
	tiles.prev->next = tile;
	tile->next = &tiles;
	tile->prev = tiles.prev;
	tiles.prev = tile;

	return tile;
}

void W_RemoveTile(tile_t *tile)
{
	tile->next->prev = tile->prev;
	tile->prev->next = tile->next;
	free(tile);
}

void W_FreeWorld(void)
{
	tile_t *tile = tiles.next;
	
	while (tile != &tiles)
	{		
		tile_t *tile2 = tile->next;
		W_RemoveTile(tile);
		tile = tile2;
	}
	
	free(&tile_gfx);
	free(tileattr.type);
	free(tileattr.layer);
}

#define EDIT_OFFX 0
#define EDIT_OFFY 92
#define HEADER_OFF 12

void W_DrawTileLayer(uint8_t layer)
{
	if (!world_edit) {
		tile_t *tile = tiles.next;
		
		while (tile != &tiles)
		{	
			if (tileattr.layer[tile->id] != layer) { // it's not the layer we want
				tile = tile->next;
				continue;
			}
			
			if ((tile->x + tile_width - camera.x < 0)
				|| (tile->x - camera.x > VID_WIDTH) 
				|| (tile->y + tile_height - camera.y < 0)
				|| (tile->y - camera.y > VID_HEIGHT))
			{ // it's not on the screen
				tile = tile->next;
				continue;
			}
					
			V_DrawCropped(
				tile_gfx, // tileset
				(tile->x - camera.x), // x pos
				(tile->y - camera.y), // y pos
				(tile->id % tileattr.tiles_per_row) * tile_width, // tileset start x pos
				(tile->id / tileattr.tiles_per_row) * tile_height, // tileset start y pos
				tile_width, // width
				tile_height, // height
				0 // flags mraoww
			);

			tile = tile->next;
		}
	} else {
		for (uint32_t i = 0; i < world_width*world_height; i++)
		{
			if (tileattr.layer[temp_world_data[i]] != layer)
				continue;
			
			uint16_t x = (i % world_width)*tile_width + EDIT_OFFX;
			uint16_t y = (i / world_width)*tile_height + EDIT_OFFY + HEADER_OFF;
			
			if ((x + tile_width - camera.x < 0)
				|| (x - camera.x > VID_WIDTH) 
				|| (y + tile_height - camera.y <= EDIT_OFFY + HEADER_OFF)
				|| (y - camera.y > VID_HEIGHT))
				continue;
			
			V_DrawCropped(
				tile_gfx, // tileset
				(x - camera.x), // x pos
				(y - camera.y), // y pos
				(temp_world_data[i] % tileattr.tiles_per_row) * tile_width, // tileset start x pos
				(temp_world_data[i] / tileattr.tiles_per_row) * tile_height, // tileset start y pos
				tile_width, // width
				tile_height, // height
				0 // flags mraoww
			);
		}
	}
}

// underwater palette map

static uint8_t water_palmap[43] = {
	00, // 00, black
	30, // 01, white
	31, // 02, grey 1
	32, // 03, grey 2
	33, // 04, grey 3
	34, // 05, grey 4
	40, // 06, carbon
	36, // 07, pink
	37, // 08, red
	38, // 09, red 2
	39, // 10, crimson 
	40, // 11, crimson 2
	24, // 12, peach
	25, // 13, mustard
	26, // 14, apricot
	27, // 15, brown
	29, // 16, shoe brown
	26, // 17, lemon
	27, // 18, lemon 2
	28, // 19, lemon 3
	37, // 20, agz 
	38, // 21, agz 2
	39, // 22, agz 3
	40, // 23, agz 4
	31, // 24, grass 
	32, // 25, grass 2
	37, // 26, green 
	38, // 27, green 2
	39, // 28, green 3
	40, // 29, green 4
	30, // 30, cyan 
	31, // 31, sky 
	32, // 32, sky 2
	33, // 33, sky 3
	34, // 34, sky 4
	31, // 35, cloud
	31, // 36, cloud 2
	32, // 37, cloud 3
	33, // 38, cloud 4
	34, // 39, purple
	40, // 40, purple 2
	32, // 41, metal
	30, // 42, magenta
};

void W_DrawWaterTiles(void)
{
	tile_t *tile = tiles.next;
	
	while (tile != &tiles)
	{
		if (!(tileattr.type[tile->id] & TILE_WATER))
		{
			tile = tile->next;
			continue;
		}
		
		tile = tile->next;
	}
}

// when i get to doing water, i need to use
// vx += abs((game_tick + vy) % gfx.width/4 - gfx.width/4/2)/2;

void W_MakeTileAttrFromGfx(const char *filename, gfx_t gfx, uint16_t cell_size)
{
	FILE *attr_fp = fopen(filename, "wb+");
	uint32_t num_tiles, tiles_per_row;
	
	// calculate tiles per row and number of tiles from the gfx size and cell size
	tiles_per_row = gfx.width / cell_size;
	num_tiles = tiles_per_row * ((gfx.size / gfx.width) / cell_size);
	
	// write the number of tiles, and tiles per row
	fwrite(&num_tiles, sizeof(uint32_t), 1, attr_fp);
	fwrite(&tiles_per_row, sizeof(uint32_t), 1, attr_fp);

	// make sure it's a dummy value of 0
	uint32_t value = 0;
	
	// write the attributes
	for (int i = 0; i < num_tiles; i++)
		fwrite(&value, sizeof(uint32_t), 1, attr_fp);
	
	for (int i = 0; i < num_tiles; i++)
		fwrite(&value, sizeof(uint8_t), 1, attr_fp);
	
	// close file pointer
	fclose(attr_fp);
}

static void W_MakeTempWorld(void)
{
	FILE *fp = fopen("data/worlds/temp.wld", "wb+");
	char tilefilename[33];
	
	strncpy(tilefilename, "tech_demo_tiles", 32);

	fwrite(&world_width, sizeof(uint16_t), 1, fp);
	fwrite(&world_height, sizeof(uint16_t), 1, fp);
	
	// 32 bytes tile gfx filename (and tileattr)
	fwrite(&tilefilename, sizeof(char), 32, fp);
	
	// 4 bytes object id
	// 2 bytes x
	// 2 bytes y
	for (uint32_t i = 0; i < (world_width * world_height); i++) {
		uint32_t id = 24 * 6 - 1;
		fwrite(&id, sizeof(uint32_t), 1, fp);
	}
	
	W_MakeTileAttrFromGfx("data/tilesets/tech_demo_tiles.atr", tile_gfx, 8);
	
	fclose(fp);
}

int8_t edit_mode = 0;
int32_t curtile = 0, seltile = 0;
uint8_t global_col;

void W_InitWorldEdit(uint16_t width, uint16_t height)
{
	world_edit = true;
	world_width = width;
	world_height = height;
	
	W_MakeTempWorld();
	W_LoadWorld("data/worlds/temp.wld");
}

void W_UpdateWorldEdit(void)
{
	int8_t lr = G_ControlDown(PLAYER_ONE, CON_RIGHT, true) - G_ControlDown(PLAYER_ONE, CON_LEFT, true);
	int8_t ud = G_ControlDown(PLAYER_ONE, CON_DOWN, true) - G_ControlDown(PLAYER_ONE, CON_UP, true);
	
	if (lr != 0)
	{
		switch (edit_mode) {
			case 0:
				if (G_ControlDown(PLAYER_ONE, CON_A, false)) {
					seltile += lr;
				} else {
					curtile += lr;
				}
				break;
			case 1:
				if (G_ControlDown(PLAYER_ONE, CON_A, false)) {
					tileattr.type[seltile] += lr;
				} else {
					seltile += lr;
				}
				break;
		}
	}
	
	if (ud != 0)
	{
		switch (edit_mode) {
			case 0:
				if (G_ControlDown(PLAYER_ONE, CON_A, false)) {
					seltile += ud*tileattr.tiles_per_row;
				} else {
					curtile += ud*world_width;
				}
				break;
			case 1:
				if (G_ControlDown(PLAYER_ONE, CON_A, false)) {
					tileattr.type[seltile] += ud*16;
				} else {
					seltile += ud*tileattr.tiles_per_row;
				}
				break;
		}
	}
	
	if (seltile >= tileattr.num_tiles)
		seltile = tileattr.num_tiles - 1;
	if (seltile < 0)
		seltile = 0;
	
	if (tileattr.type[seltile] > TILE_MAX-1)
		tileattr.type[seltile] = TILE_MAX-1;
	if (tileattr.type[seltile] < 0)
		tileattr.type[seltile] = 0;
	
	if (curtile >= world_width * world_height)
		curtile = world_width * world_height - 1;
	if (curtile < 0)
		curtile = 0;
	
	int32_t curtile_x = (curtile % world_width) * tile_width;
	int32_t curtile_y = (curtile / world_width) * tile_height;
	
	if (curtile_x - camera.x >= VID_WIDTH)
		camera.x += tile_width;
	if (curtile_x - camera.x < 0)
		camera.x -= tile_width;
	
	if (curtile_y - camera.y + EDIT_OFFY + HEADER_OFF >= VID_HEIGHT) 
		camera.y += tile_height;
	if (curtile_y - camera.y < 0) 
		camera.y -= tile_height;

	if (G_ControlDown(PLAYER_ONE, CON_C, false) && edit_mode == 0)
		temp_world_data[curtile] = seltile;
	
	if (temp_world_data[curtile] >= tileattr.num_tiles)
		temp_world_data[curtile] = tileattr.num_tiles - 1;
	if (temp_world_data[curtile] < 0)
		temp_world_data[curtile] = 0;
}

static void W_DrawWorldEditMode(void)
{
	// draw a unique grid pattern behind all tiles
	for (int i = (EDIT_OFFY+HEADER_OFF)*VID_WIDTH; i < VID_HEIGHT*VID_WIDTH; i++)
		vid.buffer[i] = palette[1 + i % 3];
	
	// draw the world
	for (uint8_t l = 0; l < 5; l++)
		W_DrawTileLayer(l);
	
	// draw the tileset
	for (int i = 0; i < tileattr.num_tiles; i++) {
		int xpos = (i % tileattr.tiles_per_row)*tile_width, ypos = (i / tileattr.tiles_per_row)*tile_height;
		V_DrawCropped(tile_gfx, 2 + xpos + (i % tileattr.tiles_per_row)*2, 2 + ypos + (i / tileattr.tiles_per_row)*2 + HEADER_OFF, xpos, ypos, tile_width, tile_height, 0);
	}
	
	// world tile pos stuff
	int world_curx = EDIT_OFFX + (curtile % world_width)*tile_width - camera.x;
	int world_cury = EDIT_OFFY + (curtile / world_width)*tile_height - camera.y;
	
	// draw world cursor pos
	V_DrawText(va("%03d, %03d", (curtile % world_width), (curtile / world_width)), VID_WIDTH - 48, EDIT_OFFY+HEADER_OFF-16, 0);
	V_DrawText(va("%03d, %03d", camera.x/8, camera.y/8), VID_WIDTH - 48, EDIT_OFFY+HEADER_OFF-8, 0);
	
	// tileset pos stuff
	int tile_curxpos = (seltile % tileattr.tiles_per_row)*tile_width, tile_curypos = (seltile / tileattr.tiles_per_row)*tile_height;
	tile_curxpos = 2 + tile_curxpos + (seltile % tileattr.tiles_per_row)*2;
	tile_curypos = 2 + tile_curypos + (seltile / tileattr.tiles_per_row)*2;
	
	// draw a box over the currently selected tile in the tileset
	for (int x = 0; x < tile_width+2; x++) {
		V_DrawDot(tile_curxpos-1+x, HEADER_OFF + tile_curypos-1, global_col);
		V_DrawDot(tile_curxpos-1+x, HEADER_OFF + tile_curypos+tile_height, global_col);
	}
	
	for (int y = 0; y < tile_height+2; y++) {
		V_DrawDot(tile_curxpos-1, HEADER_OFF + tile_curypos-1+y, global_col);
		V_DrawDot(tile_curxpos+tile_width, HEADER_OFF + tile_curypos-1+y, global_col);
	}
	
	// draw an overlay on the currently highlighted world tile
	for (int y = 0; y < tile_height; y++)
		for (int x = 0; x < tile_width; x++)
			for (int p = 0; p < 43; p++) {
				int pix = vid.buffer[world_curx + world_cury*VID_WIDTH + x + y*VID_WIDTH + HEADER_OFF*VID_WIDTH];
				if (pix == palette[p])
					vid.buffer[world_curx + world_cury*VID_WIDTH + x + y*VID_WIDTH + HEADER_OFF*VID_WIDTH] = palette[water_palmap[p]];
			}
}

static void W_DrawTileEditMode(void)
{
	// draw the tileset
	for (int i = 0; i < tileattr.num_tiles; i++) {
		int xpos = (i % tileattr.tiles_per_row)*tile_width, ypos = (i / tileattr.tiles_per_row)*tile_height;
		V_DrawCropped(tile_gfx, 2 + xpos + (i % tileattr.tiles_per_row)*2, 2 + ypos + (i / tileattr.tiles_per_row)*2 + HEADER_OFF, xpos, ypos, tile_width, tile_height, 0);
	}
	
	// tileset pos stuff
	int tile_curxpos = (seltile % tileattr.tiles_per_row)*tile_width, tile_curypos = (seltile / tileattr.tiles_per_row)*tile_height;
	tile_curxpos = 2 + tile_curxpos + (seltile % tileattr.tiles_per_row)*2;
	tile_curypos = 2 + tile_curypos + (seltile / tileattr.tiles_per_row)*2;
	
	// draw a box over the currently selected tile in the tileset
	for (int x = 0; x < tile_width+2; x++) {
		V_DrawDot(tile_curxpos-1+x, HEADER_OFF + tile_curypos-1, global_col);
		V_DrawDot(tile_curxpos-1+x, HEADER_OFF + tile_curypos+tile_height, global_col);
	}
	
	for (int y = 0; y < tile_height+2; y++) {
		V_DrawDot(tile_curxpos-1, HEADER_OFF + tile_curypos-1+y, global_col);
		V_DrawDot(tile_curxpos+tile_width, HEADER_OFF + tile_curypos-1+y, global_col);
	}
	
	// current tile type
	V_DrawText(va("tile type: %d", tileattr.type[seltile]), tileattr.tiles_per_row*(tile_width+4) - 10, 2 + HEADER_OFF, 0);
	
	if (tileattr.type[seltile] & TILE_SOLID)
		V_DrawText("O solid", 6 + tileattr.tiles_per_row*(tile_width+4) + 16, 16 + HEADER_OFF, 0);
	else
		V_DrawText("X solid", 6 + tileattr.tiles_per_row*(tile_width+4) + 16, 16 + HEADER_OFF, 0);
	
	if (tileattr.type[seltile] & TILE_SEMI)
		V_DrawText("O semi", 6 + tileattr.tiles_per_row*(tile_width+4) + 16, 26 + HEADER_OFF, 0);
	else
		V_DrawText("X semi", 6 + tileattr.tiles_per_row*(tile_width+4) + 16, 26 + HEADER_OFF, 0);
	
	if (tileattr.type[seltile] & TILE_CRUMBLE)
		V_DrawText("O crumb", 6 + tileattr.tiles_per_row*(tile_width+4) + 16, 36 + HEADER_OFF, 0);
	else
		V_DrawText("X crumb", 6 + tileattr.tiles_per_row*(tile_width+4) + 16, 36 + HEADER_OFF, 0);
	
	if (tileattr.type[seltile] & TILE_WATER)
		V_DrawText("O water", 6 + tileattr.tiles_per_row*(tile_width+4) + 16, 46 + HEADER_OFF, 0);
	else
		V_DrawText("X water", 6 + tileattr.tiles_per_row*(tile_width+4) + 16, 46 + HEADER_OFF, 0);
	
	if (tileattr.type[seltile] & TILE_MOVING)
		V_DrawText("O movin'", 6 + tileattr.tiles_per_row*(tile_width+4) + 16, 56 + HEADER_OFF, 0);
	else
		V_DrawText("X movin'", 6 + tileattr.tiles_per_row*(tile_width+4) + 16, 56 + HEADER_OFF, 0);
}

static void W_DrawFileEditMode(void)
{
	
}


void W_DrawWorldEdit(void)
{
	global_col = abs((I_GetTicks()/10) % 4 - 2) + 31; 
	
	// draw edit header
	uint8_t header_highlight_x, header_highlight_w;
	
	V_DrawText("World", 4, 2, 0);
	V_DrawText("Tile", VID_WIDTH/2 - 10, 2, 0);
	V_DrawText("File", VID_WIDTH - 26, 2, 0);
	
	if (G_ControlDown(PLAYER_ONE, CON_X, true))
		edit_mode--;
	if (G_ControlDown(PLAYER_ONE, CON_Y, true))
		edit_mode++;
	
	if (edit_mode > 2)
		edit_mode = 2;
	if (edit_mode < 0)
		edit_mode = 0;
	
	switch (edit_mode) {
		case 0:
			header_highlight_x = 4;
			header_highlight_w = 29;
			W_DrawWorldEditMode();
			break;
		case 1:
			header_highlight_x = VID_WIDTH/2 - 10;
			header_highlight_w = 19;
			W_DrawTileEditMode();
			break;
		case 2:
			header_highlight_x = VID_WIDTH - 26;
			header_highlight_w = 19;
			W_DrawFileEditMode();
			break;
	}
	
	for (int x = header_highlight_x; x < header_highlight_x + header_highlight_w; x++)
		V_DrawDot(x, 10, global_col);
}

void W_SaveWorld(const char* filename)
{
	
}
