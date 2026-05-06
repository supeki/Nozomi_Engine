// JADEFRACTURE
// game_world.c

uint32_t num_worlds = 1;
world_t *worlds;

typedef enum
{
	GRENCOVE
	GRENPLAN,
	GRFOREST,
	CRIMSCAN,
	MOLEMINE,
	FROSTFOR,
	ICEPALAC,
	SUNKENPL,
	OPNOCEAN,
	JADEREAC,
	AERGARDE,
	ARCREALM,
	NUMBASESETS
} basesets_e

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
void W_InitWorlds(void)
{
	memset(worlds, 0, num_worlds * sizeof(world_t));
	
	// load the default game worlds here later
}

// Load a tileset with tile type data and a filename for graphics data
// Tileset format
// 8 bytes for GFX filename
// 256 bytes for Tile types
tileset_t W_LoadTileset(const char *filename)
{
	return (tileset_t)NULL;
}

// Load a world from a .wld file or something
// World format
// 2 bytes for World ID
// 8 bytes for Tileset filename
// 1 Byte for Number of Layers
// 4 bytes for World size
// <9 * World size * Number of Layers> bytes for Tiles
// Tile format:
// T    XXXX       YYYY
// Type X Position Y Position
// Tile Layers are assumed to be in order.
void W_LoadWorld(const char *filename)
{
	FILE *file = fopen(filename, "rb");
	uint32_t world_id;
	char tileset_name[8];
	uint8_t num_layers;

	// check file
	if (!file)
	{
		I_Error("Failed to open %s!\n", filename);
	}
	
	// get the world id
	fread(&world_id, sizeof(uint32_t), 1, file);
	
	// get the tileset name
	for (int i = 0; i < 8; i++)
		fread(&tileset_name[i], 1, 1, file);
	
	// get the number of tile layers
	fread(&num_layers, sizeof(uint8_t), 1, file);
	
	
}

// Debug function only so I can make worlds and save them to a file from within the game itself.
void W_SaveWorld(const char *filename)
{
	
}