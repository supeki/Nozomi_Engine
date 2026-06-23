// Nozomi Engine
// game_world.c

#include "i_system.h"

#include "game_defs.h"
#include "game_world.h"

tile_t tiles;
tileset_t tileset;

typedef enum
{
	TS_GRENCOVE,
	TS_GRENPLAN,
	TS_GRFOREST,
	TS_CRIMSCAN,
	TS_MOLEMINE,
	TS_FROSTFOR,
	TS_ICEPALAC,
	TS_SUNKENPL,
	TS_OPNOCEAN,
	TS_JADEREAC,
	TS_AERGARDE,
	TS_ARCREALM,
	NUMBASESETS
} basesets_e;

const char *basesets[NUMBASESETS] = {
	"GRENCOVE",
	"GRENPLAN",
	"GRFOREST",
	"CRIMSCAN",
	"MOLEMINE",
	"FROSTFOR",
	"ICEPALAC",
	"SUNKENPL",
	"OPNOCEAN",
	"JADEREAC",
	"AERGARDE",
	"ARCREALM"
};

// Ran in gameMain to initialize stuff.
void W_InitWorld(void)
{
	// load the test world here
	W_LoadWorld("data/worlds/test.wld");
}

// Load a tileset with tile type data and a filename for graphics data
// Tileset format
// 8 bytes for GFX filename
// 1 byte for Number of Tiles
// <Number of Tiles> bytes for Tile Types
void W_LoadTileset(const char *filename)
{
	FILE *file = fopen(filename, "rb");
	char gfx_name[9];
	uint8_t num_tiles;
	
	// check file
	if (!file)
	{
		I_Error("Failed to open %s!\n", filename);
	}
	
	// get the gfx name
	for (int i = 0; i < 8; i++)
		fread(&gfx_name[i], sizeof(char), 1, file);
	
	// load the gfx
	tileset.gfx = GFX_LoadGFX(va("data/tilesets/%s.gfx", gfx_name));
	
	// get the number of tiles
	fread(&num_tiles, sizeof(uint8_t), 1, file);
	
	// allocate data for tile types
	tileset.data = nMalloc(num_tiles*sizeof(uint8_t));
	
	for (int i = 0; i < num_tiles; i++)
		fread(&tileset.data[i], sizeof(uint8_t), 1, file);
}

// Load a world from a .wld file or something
// World format
// 8 bytes for Tileset filename
// 1 Byte for Number of Layers
// 4 bytes for World size
// <5 * World size * Number of Layers> bytes for Tiles
// Tile format:
// T    XX         YY
// Type X Position Y Position
// Tile Layers are assumed to be in order.
void W_LoadWorld(const char *filename)
{
	FILE *file = fopen(filename, "rb");
	char tileset_name[9];
	uint8_t num_layers;
	uint32_t world_size;

	// check file
	if (!file)
	{
		I_Error("Failed to open %s!\n", filename);
	}

	// free everything
	W_FreeTiles();
	W_FreeTileset();
	
	// get the tileset name
	for (int i = 0; i < 8; i++)
		fread(&tileset_name[i], sizeof(char), 1, file);
	
	// get the number of tile layers
	fread(&num_layers, sizeof(uint8_t), 1, file);
	
	// get the world size
	fread(&world_size, sizeof(uint32_t), 1, file);
	
	// read tile data and make tiles
	for (int l = 0; l < num_layers; l++)
		for (int i = 0; i < world_size; i++)
		{
			int16_t x, y;
			uint8_t id;
			
			fread(&id, sizeof(uint8_t), 1, file);
			fread(&x, sizeof(int16_t), 1, file);
			fread(&y, sizeof(int16_t), 1, file);
			
			x = TO_SUBPIXELS(x*16);
			y = TO_SUBPIXELS(y*16);
			
			W_CreateTile(x, y, id, l);
		}
}

tile_t *W_CreateTile(subpixel_t x, subpixel_t y, uint8_t id, uint8_t layer)
{
	tile_t *tile;
	tile = nMalloc(sizeof(tile_t));
	memset(tile, 0, sizeof(tile_t));
	
	tile->id = id;
	tile->x = x;
	tile->y = y;
	
	// add this tile to the list of tiles
	tiles.prev->next = tile;
	tile->next = &tiles;
	tile->prev = tiles.prev;
	tiles.prev = tile;

	return tile;
}

// Frees all Tiles in the World
void W_FreeTiles(void)
{
	tile_t *tile = tiles.next;
	
	while (tile != &tiles)
	{
		tile_t *tile2 = tile->next;
		
		tile->next->prev = tile->prev;
		tile->prev->next = tile->next;
		free(tile);

		tile = tile2;
	}
}

// Free all tileset data
void W_FreeTileset(void)
{
	if (tileset.data)
		free(tileset.data);
	
	if (&tileset.gfx != NULL)
		free(&tileset.gfx);
}

// Debug function only so I can make worlds 
// and save them to a file from within the game itself.
void W_SaveWorld(const char *filename)
{
	
}