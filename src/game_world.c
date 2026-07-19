// Nozomi Engine
// game_world.c

#include "i_system.h"

#include "game_defs.h"
#include "game_object.h"
#include "game_world.h"

uint16_t world_width, world_height;
uint16_t tile_width, tile_height;
tile_t tiles;
tileattr_t tileattr;
gfx_t tile_gfx;

void W_InitTiles(void)
{
	tiles.prev = tiles.next = &tiles;
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
	attr_fp = fopen(va("data/tilesets/%s.atr", tilefilename), "rb");
	
	// tile attributes
	fread(&tileattr.num_tiles, sizeof(uint32_t), 1, attr_fp);
	fread(&tileattr.tiles_per_row, sizeof(uint32_t), 1, attr_fp); // used for gfx when drawing
	
	tile_height = tile_width = (tile_gfx.width / tileattr.num_tiles); // needs to be divisible by 2 and square ratio?
	
	tileattr.type =  malloc(tileattr.num_tiles * sizeof(uint32_t));
	tileattr.layer = malloc(tileattr.num_tiles * sizeof(uint8_t));
	
	for (int i = 0; i < tileattr.num_tiles; i++)
		fread(&tileattr.type[i], sizeof(uint32_t), 1, attr_fp);
	for (int i = 0; i < tileattr.num_tiles; i++)
		fread(&tileattr.layer[i], sizeof(uint8_t), 1, attr_fp);
	
	fclose(attr_fp);
	
	// 4 bytes object id
	// 2 bytes x
	// 2 bytes y
	for (uint32_t i = 0; i < (world_width * world_height); i++) {
		uint32_t id;
		uint16_t x, y;
		
		fread(&id, sizeof(uint32_t), 1, fp);
		fread(&x, sizeof(uint16_t), 1, fp);
		fread(&y, sizeof(uint16_t), 1, fp);
		
		W_CreateTile(TO_SUBPIXELS(x)*16, TO_SUBPIXELS(y)*16, id);
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

void W_DrawTileLayer(uint8_t layer)
{
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
			|| (tile->y - camera.y > VID_HEIGHT) )
		{ // it's not on the screen
			tile = tile->next;
			continue;
		}
		
		V_DrawCropped(
			tile_gfx, // tileset
			TO_PIXELS(tile->x - camera.x), // x pos
			TO_PIXELS(tile->y - camera.y), // y pos
			(tile->id % tileattr.tiles_per_row) * tile_width, // tileset start x pos
			(tile->id / tileattr.tiles_per_row) * tile_height, // tileset start y pos
			tile_width, // width
			tile_height, // height
			0 // flags mraoww
		);

		tile = tile->next;
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

void W_SaveWorld(const char* filename)
{
	
}
