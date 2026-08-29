// Nozomi Engine
// game_world.c

#include "game_object.h"
#include "game_video.h"
#include "game_world.h"

#include "i_system.h"

uint8_t tile_width, tile_height; // 256x256 is pretty big for one tile as-is
uint32_t *tile_attributes; // per-tile attributes (basically just flags)
uint32_t num_tiles; // for safe keeping

uint16_t world_width, world_height; // shouldn't need larger than 65536x65536 tiles right
uint32_t world_bgtype; // background type maybe if i wanna have a 2d game with cool backgrounds (like cave story)
uint16_t *world_tiles = NULL, *world_bgtiles = NULL; // tile layout in the world

gfx_t gfx_tileset, gfx_worldbg;

static void W_LoadTileset(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    char graphic_name[33];
    uint32_t i;

    // load tileset graphics
    fread(&graphic_name, sizeof(char), 32, fp);
    graphic_name[32] = '\0';

    if (&gfx_tileset)
        GFX_FreeGFX(&gfx_tileset);

    gfx_tileset = GFX_LoadGFX(va("data/tilesets/%s.bmp", graphic_name));

    // load tileset data
    // get tile width/height
    fread(&tile_width, sizeof(uint8_t), 1, fp);
    fread(&tile_height, sizeof(uint8_t), 1, fp);

    // get number of tiles from graphic size and tile width/height
    num_tiles = (gfx_tileset.width / tile_width) * (gfx_tileset.height / tile_height);

    // get tile attributes from file
    tile_attributes = malloc(num_tiles * sizeof(uint32_t));
    for (i = 0; i < num_tiles; i++)
        fread(&tile_attributes[i], sizeof(uint32_t), 1, fp);

    fclose(fp);
}

void W_LoadWorldFile(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    char tileset_name[33];
    char background_name[33];
    uint32_t i, num_objs;

    // free prior stuff
    W_Free();

    // read world header

    // load tileset
    fread(&tileset_name, sizeof(char), 32, fp);
    tileset_name[32] = '\0';

    W_LoadTileset(va("data/tilesets/%s.set", tileset_name));

    // get world width and height
    fread(&world_width, sizeof(uint16_t), 1, fp);
    fread(&world_height, sizeof(uint16_t), 1, fp);

    // read world data
    fread(&world_bgtype, sizeof(uint32_t), 1, fp);

    // load background if not using tiles
    if ((world_bgtype & BG_TILE) == 0) {
        fread(&background_name, sizeof(char), 32, fp);
        background_name[32] = '\0';

        if (&gfx_worldbg)
            GFX_FreeGFX(&gfx_worldbg);

        gfx_worldbg = GFX_LoadGFX(va("data/backgrounds/%s.bmp", background_name));
    }

    // read world tiles
    world_tiles = malloc(world_width * world_height * sizeof(uint16_t));
    for (i = 0; i < world_width * world_height; i++)
        fread(&world_tiles[i], sizeof(uint16_t), 1, fp);

    // load background tiles if using tiles
    if (world_bgtype & BG_TILE) {
        world_bgtiles = malloc(world_width * world_height * sizeof(uint16_t));

        for (i = 0; i < world_width * world_height; i++)
            fread(&world_bgtiles[i], sizeof(uint16_t), 1, fp);
    }

    // load objects into world
    fread(&num_objs, sizeof(uint32_t), 1, fp); // i'm lazyy so i'll store num of objs in file

    for (i = 0; i < num_objs; i++) {
        object_t *obj;
        uint32_t type, x, xoff, y, yoff, flags;
        uint8_t dir;

        fread(&type, sizeof(uint32_t), 1, fp);
        fread(&x, sizeof(uint32_t), 1, fp);
        fread(&xoff, sizeof(uint32_t), 1, fp);
        fread(&y, sizeof(uint32_t), 1, fp);
        fread(&yoff, sizeof(uint32_t), 1, fp);
        fread(&dir, sizeof(uint8_t), 1, fp);
        fread(&flags, sizeof(uint32_t), 1, fp);

        obj = OBJ_CreateObject(x*PU + xoff, y*PU + yoff, type);
        obj->dir = dir;
        obj->flags = flags;
    }

    fclose(fp);
}

void W_Free(void)
{
    if (world_tiles != NULL) {
        free(world_tiles);
        world_tiles = NULL;
    }

    if (world_bgtiles != NULL) {
        free(world_bgtiles);
        world_bgtiles = NULL;
    }

    if (tile_attributes != NULL) {
        free(tile_attributes);
        tile_attributes = NULL;
    }
}

void W_CreateTilesetFromFile(const char *input, const char *output, uint8_t tile_size)
{
    FILE *fp = fopen(va("data/tilesets/%s.set", output), "wb+");
    gfx_t gfx_temptileset;
    uint32_t i, num_temptiles = 0, dummy = 0;
    char gfx_name[33];

    // gfx name for writing to file,,
    sprintf(gfx_name, "%s", input);

    // load graphics and write name to file
    gfx_temptileset = GFX_LoadGFX(va("data/tilesets/%s.bmp", input));
    fwrite(gfx_name, sizeof(char), 32, fp);

    fwrite(&tile_size, sizeof(uint8_t), 1, fp); // write this twice since
    fwrite(&tile_size, sizeof(uint8_t), 1, fp); // my tiles are squares

    // get number of tiles from graphic size and tile width/height
    num_temptiles = (gfx_temptileset.width / tile_size) * (gfx_temptileset.height / tile_size);

    // write dummy attrs
    for (i = 0; i < num_temptiles; i++)
        fwrite(&dummy, sizeof(uint32_t), 1, fp);

    fclose(fp);
}

bool tileset_edit = false; // tileset editing dawg
char temp_tile_name[33]; // save temp tileset to this name
uint8_t tiles_per_row = 0, tile_per_gfx_row = 0; // tiles to display per-row in editor

// one or the other
void W_StartTilesetEdit(const char *gfx_name, const char *tileset_name)
{
    if (gfx_name == NULL && tileset_name == NULL)
    {
        I_printf("Expected tileset name or gfx name to load.\n");
        tileset_edit = false;
        return;
    }

    if (gfx_name == NULL) { // didn't specify a graphics file to base from
        W_LoadTileset(va("data/tilesets/%s.set", tileset_name)); // so you must want to edit a pre-made file
        sprintf(temp_tile_name, tileset_name);
    } else if (tileset_name == NULL) { // no tileset file
        W_CreateTilesetFromFile(gfx_name, gfx_name, 8); // so you're ok with
        W_LoadTileset(va("data/tilesets/%s.set", gfx_name)); // the graphics name being used instead
        sprintf(temp_tile_name, gfx_name);
    } else { // specified both
        W_CreateTilesetFromFile(gfx_name, tileset_name, 8); // so make the tileset file
        W_LoadTileset(va("data/tilesets/%s.set", tileset_name)); // with the graphics but your preferred name
        sprintf(temp_tile_name, tileset_name);
    }

    // calculate tiles per row now
    // also calculate tiles per row for the graphics since it could be less
    tiles_per_row = VID_WIDTH / (tile_width + 2); // adding 2 for left/right padding
    tile_per_gfx_row = gfx_tileset.width / (tile_width + 2);

    tileset_edit = true; // we're tileset editing now dawg
}

void W_UpdateTilesetEdit(void)
{
    
}

void W_DrawTilesetEdit(void)
{
    int i;

    // draw tiles first
    for (i = 0; i < num_tiles; i++)
        V_DrawCropped(
            gfx_tileset, // gfx
            1 + (i % tiles_per_row) * (tile_width + 1), // x 
            1 + (i / tiles_per_row) * (tile_height + 1), // y
            (i % tile_per_gfx_row) * tile_width, // crop x
            (i / tile_per_gfx_row) * tile_height, // crop y
            tile_width, // crop w
            tile_height, // crop h
            0 // flags
        );
}