// Nozomi Engine
// game_world.c

#include "game_world.h"

uint8_t tile_width, tile_height; // 256x256 is pretty big for one tile as-is
uint32_t *tile_attributes; // per-tile attributes (basically just flags)

uint16_t world_width, world_height; // shouldn't need larger than 65536x65536 tiles right
uint32_t world_bgtype; // background type maybe if i wanna have a 2d game with cool backgrounds (like cave story)
uint16_t *world_tiles = NULL, *world_bgtiles = NULL; // tile layout in the world

gfx_t gfx_tileset, gfx_worldbg;

void W_LoadTileset(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    char graphic_name[33];
    uint32_t i, num_tiles;

    // load tileset graphics
    fread(&graphic_name, sizeof(char), 32, fp);
    graphic_name[32] = '\0';

    if (gfx_tileset)
        GFX_FreeGFX(&gfx_tileset);

    gfx_tileset = GFX_LoadGFX(va("data/tilesets/%s.bmp", graphic_name));

    // load tileset data
    // get tile width/height
    fread(&tile_width, sizeof(uint8_t), 1, fp);
    fread(&tile_height, sizeof(uint8_t), 1, fp);

    // get number of tiles from graphic size and tile width/height
    num_tiles = (gfx_tileset.width / tile_width) * (gfx_tileset.height / tile_height);

    // free if exist already
    if (tile_attributes)
        free(tile_attributes);

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

        if (gfx_worldbg)
            GFX_FreeGFX(&gfx_worldbg);

        gfx_worldbg = GFX_LoadGFX(va("data/backgrounds/%s.bmp", background_name));
    }

    // read world tiles
    if (world_tiles) {
        free(world_tiles);
        world_tiles = NULL;
    }

    if (world_bgtiles) {
        free(world_bgtiles);
        world_bgtiles = NULL;
    }

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
        obj_t *obj
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
