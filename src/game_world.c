// Nozomi Engine
// game_world.c

#include "game_input.h"
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

static void W_SaveTileset(const char *name)
{
    FILE *fp = fopen(va("data/tilesets/%s.set", name), "wb+");
    gfx_t gfx_temptileset;
    uint32_t i;
    char gfx_name[33];

    sprintf(gfx_name, "%s", name);
    fwrite(gfx_name, sizeof(char), 32, fp);
    fwrite(&tile_width, sizeof(uint8_t), 1, fp);
    fwrite(&tile_height, sizeof(uint8_t), 1, fp);

    // write attrs
    for (i = 0; i < num_tiles; i++)
        fwrite(&tile_attributes[i], sizeof(uint32_t), 1, fp);

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

void W_CreateTilesetFromFile(const char *input, uint8_t tile_size)
{
    uint32_t i;

    gfx_tileset = GFX_LoadGFX(va("data/tilesets/%s.bmp", input));

    tile_width = tile_height = tile_size;
    num_tiles = (gfx_tileset.width / tile_width) * (gfx_tileset.height / tile_height);
    
    tile_attributes = malloc(num_tiles * sizeof(uint32_t));
    for (i = 0; i < num_tiles; i++)
        tile_attributes[i] = 0;
}

bool tileset_edit = false; // tileset editing dawg
static char temp_tile_name[33]; // save temp tileset to this name
static uint8_t tiles_per_row = 0; // tiles to display per-row in editor
static int32_t current_tile = 0; // int instead of uint for some silly things

static int16_t tile_screenw = 15;
static int16_t tile_screenh = 11;
static bool tile_selected = false;
static uint8_t tile_sel_option = 0;
static bool file_menu = false;
static dirfiles_t tileset_dirfiles;
static char *last_path;

static void W_UpdateTilesetFiles(dirfiles_t *dirfiles, const char *path)
{
    if (dirfiles != NULL)
        DF_Free(dirfiles);

    *dirfiles = I_GetDir(path);

    if (last_path != NULL) {
        free(last_path);
        last_path = NULL;
    }

    last_path = malloc((strlen(path)+1) * sizeof(char));
    strcpy(last_path, path);
}

// one or the other
void W_StartTilesetEdit(const char *gfx_name, const char *tileset_name)
{
    if (gfx_name == NULL && tileset_name == NULL)
    {
        tileset_edit = true;
        file_menu = true;
        tile_sel_option = 0;
        W_UpdateTilesetFiles(&tileset_dirfiles, va("%s/data/tilesets", I_GetHomeDir()));
        return;
    }

    if (gfx_name == NULL) { // didn't specify a graphics file to base from
        W_LoadTileset(va("data/tilesets/%s.set", tileset_name)); // so you must want to edit a pre-made file
        sprintf(temp_tile_name, tileset_name);
    } else if (tileset_name == NULL) { // no tileset file
        W_CreateTilesetFromFile(gfx_name, 8); // so you're ok with the
        sprintf(temp_tile_name, gfx_name); // graphics name being used instead
    } else { // specified both
        W_CreateTilesetFromFile(gfx_name, 8); // so make the tileset file with the
        sprintf(temp_tile_name, tileset_name); // graphics but your preferred name
    }

    // calculate tiles per row now
    tiles_per_row = gfx_tileset.width / tile_width; // adding 2 for left/right padding
    current_tile = 0;
    tile_screenw = (((16 * PU) / tile_width) * 8)/PU - 1;
    tile_screenh = (((12 * PU) / tile_height) * 8)/PU - 1;
    tile_selected = false;
    tile_sel_option = 0;

    file_menu = false;
    tileset_edit = true; // we're tileset editing now dawg
}

void W_UpdateTilesetEdit(void)
{  
    if (file_menu) {
        if (G_ControlDown(PLAYER_ONE, CON_START, true)) {
            file_menu = false;
            return;
        }

        if (tileset_dirfiles.num_files < 1)
            return; // no files so no loading anything

        if (G_ControlDown(PLAYER_ONE, CON_UP, true))
            tile_sel_option--;
        if (G_ControlDown(PLAYER_ONE, CON_DOWN, true))
            tile_sel_option++;
        if (G_ControlDown(PLAYER_ONE, CON_LEFT, true))
            tile_sel_option-=16;
        if (G_ControlDown(PLAYER_ONE, CON_RIGHT, true))
            tile_sel_option+=16;

        // sanity checks
        if (tile_sel_option < 0)
            tile_sel_option = tileset_dirfiles.num_files - abs(tile_sel_option);

        // still below zero?
        if (tile_sel_option < 0)
            tile_sel_option = 0; // sigh..
        
        // loop around if need be
        tile_sel_option = tile_sel_option % tileset_dirfiles.num_files;

        if (G_ControlDown(PLAYER_ONE, CON_A, true)) {
            char *dot;
            char *name;
            const char *filename;
            int name_len;

            filename = tileset_dirfiles.filenames[tile_sel_option];
            dot = strrchr(filename, '.');

            name_len = strlen(filename) + 1;
            name = malloc((name_len-4) * sizeof(char)); 
            snprintf(name, name_len-4, "%s", filename);

            I_printf(name);

            if (!strcmp(dot, ".bmp")) {
                W_StartTilesetEdit(name, NULL);
            } else {
                W_StartTilesetEdit(NULL, name);
            }

            free(name);
        }

        // return early
        return;
    }

    if (G_ControlDown(PLAYER_ONE, CON_START, true)) {
        W_StartTilesetEdit(NULL, NULL); // open file menu
        return;
    }

    if (num_tiles <= 0) {
        return;
    }

    if (!tile_selected) {
        if (G_ControlDown(PLAYER_ONE, CON_UP, true))
            current_tile-=tiles_per_row;
        if (G_ControlDown(PLAYER_ONE, CON_DOWN, true))
            current_tile+=tiles_per_row;
        if (G_ControlDown(PLAYER_ONE, CON_LEFT, true))
            current_tile--;
        if (G_ControlDown(PLAYER_ONE, CON_RIGHT, true))
            current_tile++;
    } else if (G_ControlDown(PLAYER_ONE, CON_B, true)) {
        tile_selected = false;
        tile_sel_option = 0;
    }

    // sanity checks
    if (current_tile < 0)
        current_tile = num_tiles - abs(current_tile);

    // still below zero?
    if (current_tile < 0)
        current_tile = 0; // sigh..
     
    // loop around if need be
    current_tile = current_tile % num_tiles;

    // now that sanity checks are done, we can change the rest of the stuff
    if (tile_selected) {
        if (G_ControlDown(PLAYER_ONE, CON_UP, true))
            tile_sel_option--;
        if (G_ControlDown(PLAYER_ONE, CON_DOWN, true))
            tile_sel_option++;

        // sanity checks
        if (tile_sel_option < 0)
            tile_sel_option = TILE_OPTIONS - 1;
        
        // loop around if need be
        tile_sel_option = tile_sel_option % TILE_OPTIONS;

        if (G_ControlDown(PLAYER_ONE, CON_A, true))
            if (tile_attributes[current_tile] & (1<<tile_sel_option))
                tile_attributes[current_tile] &= ~(1<<tile_sel_option);
            else
                tile_attributes[current_tile] |= (1<<tile_sel_option);
    } else {
        if (G_ControlDown(PLAYER_ONE, CON_A, true))
            tile_selected = true;
        if (G_ControlDown(PLAYER_ONE, CON_C, true))
            W_SaveTileset(temp_tile_name);
    }
}

int16_t tile_offy = 0;
int16_t tile_offx = 0;

// my super yucky drawing code
void W_DrawTilesetEdit(void)
{
    int i, cur_posx, cur_posy;

    if (file_menu) {
        V_DrawText("Nozomi Engine Tileset Editor\nSelect a file to open:", 0, 0, 0);

        if (tileset_dirfiles.num_files > 0) {
            for (i = 0; i < tileset_dirfiles.num_files; i++)
                V_DrawText(va("%s", tileset_dirfiles.filenames[i]), (i/17) * 80 + 8, (i%17) * 10 + 20, 0);

            V_DrawText(">", (tile_sel_option/17) * 80 + 1, (tile_sel_option%17) * 10 + 20, 0);
        } else
            V_DrawText("No files found.", 8, 20, 0);

        V_DrawText("Close the File Menu with Start/Enter", 0, VID_HEIGHT-8, 0);
        // return early
        return;
    }

    if (num_tiles <= 0) {
        V_DrawText("No tileset loaded, or no tiles found.\nRe-open the File Menu with Start/Enter.", 0, 0, 0);
        return;
    }

    cur_posx = 1 + (current_tile % tiles_per_row) * (tile_width+1);
    cur_posy = 1 + (current_tile / tiles_per_row) * (tile_height+1);

    if (cur_posx > (tile_screenw/2) * (tile_width+1))
        tile_offx = (cur_posx/(tile_width+1)) - (tile_screenw/2);
    else 
        tile_offx = 0;

    if (tile_offx >= tiles_per_row - tile_screenw)
        tile_offx = tiles_per_row - tile_screenw;

    if (cur_posy > (tile_screenh/2) * (tile_height+1))
        tile_offy = (cur_posy/(tile_height+1)) - (tile_screenh/2);
    else 
        tile_offy = 0;

    if (tile_offy >= num_tiles/tiles_per_row - tile_screenh)
        tile_offy = num_tiles/tiles_per_row - tile_screenh;

    // draw tiles first
    for (i = 0; i < num_tiles; i++)
    {
        if (i/tiles_per_row - tile_offy > tile_screenh-1)
            break;

        if (i%tiles_per_row - tile_offx > tile_screenw-1)
            continue;

        V_DrawCropped(
            gfx_tileset, // gfx
            2 + (i % tiles_per_row) * (tile_width + 1) - (tile_offx * (tile_width+1)), // x 
            2 + (i / tiles_per_row) * (tile_height + 1) - (tile_offy * (tile_height+1)), // y
            (i % tiles_per_row) * tile_width, // crop x
            (i / tiles_per_row) * tile_height, // crop y
            tile_width, // crop w
            tile_height, // crop h
            0 // flags
        );
    }

    V_DrawLine(tile_screenw * (tile_height + 1) + 3, 0, 180, tile_screenh * (tile_height + 1) + 4, 0xFFFFFF);
    V_DrawLine(0, tile_screenh * (tile_height + 1 ) + 3, 90, tile_screenw * (tile_height + 1) + 4, 0xFFFFFF);

    V_DrawBox(cur_posx - (tile_offx * (tile_width+1)), cur_posy - (tile_offy * (tile_height+1)), 0, tile_width + 2, tile_height + 2, 0xFFFFFF);
    V_DrawText(
        va("Tile: %03d/%03d", current_tile+1, num_tiles),
        0,
        tile_screenh * (tile_height + 1 ) + 5,
        0
    );

    /*
        TILE_NONE = 0,
        TILE_SOLID = 1, // solid tile, objects collide with it
        TILE_WATER = 2, // water tile (has water properties with objects)
        TILE_FG = 4, // foreground tile (draw in front of object)
        TILE_ANIMATED = 8
    */

    V_DrawText(va("Tile Attr:         %02d", tile_attributes[current_tile]), VID_WIDTH - 104, 10, 0);

    if (tile_attributes[current_tile] & TILE_SOLID) 
        V_DrawText("Solid", VID_WIDTH-108, 30, 0);
    else
        V_DrawText("Solid", VID_WIDTH-108, 30, 0);

    if (tile_attributes[current_tile] & TILE_WATER)
        V_DrawText("Water", VID_WIDTH-108, 42, 0);
    else
        V_DrawText("Water", VID_WIDTH-108, 42, 0);

    if (tile_attributes[current_tile] & TILE_FG)
        V_DrawText("Foreground", VID_WIDTH-108, 54, 0);
    else
        V_DrawText("Foreground", VID_WIDTH-108, 54, 0);

    if (tile_attributes[current_tile] & TILE_ANIMATED)
        V_DrawText("Animated", VID_WIDTH-108, 66, 0);
    else
        V_DrawText("Animated", VID_WIDTH-108, 66, 0);

    if (tile_attributes[current_tile] & TILE_SOLID) 
        V_DrawText("Yes", VID_WIDTH-24, 30, V_JUMPYTEXT);
    else
        V_DrawText("No", VID_WIDTH-24, 30, 0);

    if (tile_attributes[current_tile] & TILE_WATER)
        V_DrawText("Yes", VID_WIDTH-24, 42, V_JUMPYTEXT);
    else
        V_DrawText("No", VID_WIDTH-24, 42, 0);

    if (tile_attributes[current_tile] & TILE_FG)
        V_DrawText("Yes", VID_WIDTH-24, 54, V_JUMPYTEXT);
    else
        V_DrawText("No", VID_WIDTH-24, 54, 0);

    if (tile_attributes[current_tile] & TILE_ANIMATED)
        V_DrawText("Yes", VID_WIDTH-24, 66, V_JUMPYTEXT);
    else
        V_DrawText("No", VID_WIDTH-24, 66, 0);

    if (tile_selected)
        V_DrawText(">", VID_WIDTH-115 + (I_GetTicks()/15 % 2), 29 + (tile_sel_option*12), 0);

    if (tile_selected)
        V_DrawText(
            "D-Pad/Arrow Keys: Move\n" \
            "A/Y / Z: Set Attr.\n" \
            "B / X: Back\n" \
            "C: N/A",
            2,
            VID_HEIGHT - 52,
            0
        );
    else {
        V_DrawText(
            "D-Pad/Arrow Keys: Move\n" \
            "A/Y / Z: Select\n" \
            "B / X: N/A\n" \
            "C: Save Tileset",
            2,
            VID_HEIGHT - 52,
            0
        );

        V_DrawText("Press Start/Enter to open File Menu", 0, VID_HEIGHT-16, 0);
    }

    V_DrawText("Nozomi Engine Tileset Editor", 0, VID_HEIGHT-8, 0);
}