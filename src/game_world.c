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
uint16_t *world_tiles = NULL, *world_tiles2 = NULL, *world_bgtiles = NULL, *world_bgtiles2 = NULL, world_bgcolor = 0; // tile layout in the world + bg color ig

// used to be for editor only but was proven needed
static uint32_t tiles_per_row = 0;

// Q: ok maril so why is there tiles2?
// A: i was lazy and i needed to be able to layer tiles on tiles i'm sorryyyy 

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
    tiles_per_row = gfx_tileset.width / tile_width;

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
    if (world_bgtype & BG_IMAGE) {
        fread(&background_name, sizeof(char), 32, fp);
        background_name[32] = '\0';

        if (&gfx_worldbg)
            GFX_FreeGFX(&gfx_worldbg);

        gfx_worldbg = GFX_LoadGFX(va("data/backgrounds/%s.bmp", background_name));
    }

    if (world_bgtype & BG_COLOR)
        fread(&world_bgcolor, sizeof(uint16_t), 1, fp);

    // read world tiles
    world_tiles = malloc(world_width * world_height * sizeof(uint16_t));
    for (i = 0; i < world_width * world_height; i++)
        fread(&world_tiles[i], sizeof(uint16_t), 1, fp);

    world_tiles2 = malloc(world_width * world_height * sizeof(uint16_t));
    for (i = 0; i < world_width * world_height; i++)
        fread(&world_tiles2[i], sizeof(uint16_t), 1, fp);

    // load background tiles if using tiles
    world_bgtiles = malloc(world_width * world_height * sizeof(uint16_t));
    for (i = 0; i < world_width * world_height; i++)
        fread(&world_bgtiles[i], sizeof(uint16_t), 1, fp);
    
    world_bgtiles2 = malloc(world_width * world_height * sizeof(uint16_t));
    for (i = 0; i < world_width * world_height; i++)
        fread(&world_bgtiles2[i], sizeof(uint16_t), 1, fp);

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

static void W_DrawLayer(uint8_t layer) {
    uint32_t i;

    if (layer > 3) {
        // draw image or color
        if (world_bgtype & BG_IMAGE)
            V_DrawTiled(gfx_worldbg, -camera.x/4, -camera.y/4, 0); // draw the bg image and tile it
        else if ((world_bgtype & BG_COLOR) && (world_bgtype & BG_WATER) == 0)
            V_FillScreen(world_bgcolor);

        return;
    }

    for (i = 0; i < world_width*world_height; i++)
    {
        int32_t px, py;
        uint32_t id = 0;

        switch (layer) {
            case 1:
                id = world_tiles2[i]; // sub tiles
                break;
            case 2:
                id = world_bgtiles[i]; // main bg tiles
                break;
            case 3:
                id = world_bgtiles2[i]; // sub bg tiles
                break;
            default:
                id = world_tiles[i]; // main tiles
                break;
        }

        if (id == 0)
            continue;

        px = ((i % world_width) * tile_width) - camera.x;
        py = ((i / world_width) * tile_height) - camera.y;

        if (layer > 1) {
            if (world_bgtype & BG_SLOW) {
                if (layer == 2) {
                    px = ((i % world_width) * tile_width) - camera.x/3*2;
                    py = ((i / world_width) * tile_height) - camera.y/3*2;
                } else {
                    px = ((i % world_width) * tile_width) - camera.x/3;
                    py = ((i / world_width) * tile_height) - camera.y/3;
                }
            }

            if (world_bgtype & BG_STATIC) {
                px = ((i % world_width) * tile_width);
                py = ((i / world_width) * tile_height);
            }
        }

        if (world_height * tile_height < VID_HEIGHT)
            py += (VID_HEIGHT - (world_height * tile_height))/2;

        if (tile_attributes[id] & TILE_ANIMATED)
            V_DrawCroppedAnimated(
                gfx_tileset, // gfx
                px, // x 
                py, // y
                (id % tiles_per_row) * tile_width, // crop x
                (id / tiles_per_row) * tile_height, // crop y
                tile_width, // crop w
                tile_height, // crop h
                3, // frames
                3, // frames per second
                0 // flags
            );
        else
            V_DrawCropped(
                gfx_tileset, // gfx
                px, // x 
                py, // y
                (id % tiles_per_row) * tile_width, // crop x
                (id / tiles_per_row) * tile_height, // crop y
                tile_width, // crop w
                tile_height, // crop h
                0 // flags
            );
    }
}

static uint16_t *second_buf = NULL;

void W_DrawWaveEffect(uint16_t color) 
{
    uint32_t x, y;

    if (second_buf == NULL)
        second_buf = malloc(VID_WIDTH * VID_HEIGHT * sizeof(uint16_t));

    memcpy(second_buf, vid.buffer, VID_WIDTH * VID_HEIGHT * sizeof(uint16_t));
    
    for (y = 0; y < VID_HEIGHT; y++)
        for (x = 0; x < VID_WIDTH; x++) {
            int32_t sx;
            int32_t x_off = 4 * sin((I_GetTicks()+x+y) * (PI/90));

            if (x_off < -3)
                x_off = -3;
            if (x_off > 3)
                x_off = 3;

            sx = x - x_off;

            if (sx < 0)
                sx = 0;
            if (sx >= VID_WIDTH)
                sx = VID_WIDTH - 1;

            vid.buffer[x + y*VID_WIDTH] = V_MixColors(second_buf[sx + y*VID_WIDTH], color, 64);
        } 
}

void W_DrawWorld(void) {
    W_DrawLayer(4);
    W_DrawLayer(3);

    if (!(world_bgtype & BG_FG))
        W_DrawLayer(2);

    if ((world_bgtype & BG_FG) == 0 && (world_bgtype & BG_WATER))
        W_DrawWaveEffect(world_bgcolor);

    W_DrawLayer(1);
    W_DrawLayer(0);

    if (world_bgtype & BG_FG)
        W_DrawLayer(2);

    if ((world_bgtype & (BG_FG|BG_WATER)) == (BG_FG|BG_WATER))
        W_DrawWaveEffect(world_bgcolor);
}

void W_Free(void)
{
    if (world_tiles != NULL) {
        free(world_tiles);
        world_tiles = NULL;
    }

    if (world_tiles2 != NULL) {
        free(world_tiles2);
        world_tiles2 = NULL;
    }

    if (world_bgtiles != NULL) {
        free(world_bgtiles);
        world_bgtiles = NULL;
    }

    if (world_bgtiles2 != NULL) {
        free(world_bgtiles2);
        world_bgtiles2 = NULL;
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

void W_CreateWorldFromTilesetFile(const char *input, uint16_t width, uint16_t height)
{
    uint32_t i;

    W_LoadTileset(va("data/tilesets/%s.set", input));

    world_width = width;
    world_height = height;
    world_bgtype = BG_SLOW|BG_WATER|BG_COLOR;
    world_bgcolor = 0x4d5f;
    
    world_tiles = malloc(world_width * world_height * sizeof(uint16_t));
    world_tiles2 = malloc(world_width * world_height * sizeof(uint16_t));
    world_bgtiles = malloc(world_width * world_height * sizeof(uint16_t));
    world_bgtiles2 = malloc(world_width * world_height * sizeof(uint16_t));
    GFX_FreeGFX(&gfx_worldbg);

    for (i = 0; i < world_width * world_height; i++) {
        world_tiles[i] = 0; // default of the top-left tile // might want it to be transparent...
        world_tiles2[i] = 0; // blegh, draws below tiles
        world_bgtiles[i] = 0; // bleh
        world_bgtiles2[i] = 0; // bleh 2
    }
}

// idk if this needs to be here honestly, but i wanna multipurpose it now
static bool file_menu = false;
static dirfiles_t world_dirfiles, tileset_dirfiles, background_dirfiles;
static char *last_path;
static uint8_t tile_sel_option = 0;

static void W_UpdateFiles(dirfiles_t *dirfiles, const char *path)
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

bool world_edit = false; // world editing 'yo
static char temp_world_name[33]; // woa it's almost like the tile editor but now you place them

// ermm these were in the tileset editor but i'm lazy and reusing variables is cooler
static int32_t current_tile = 0; // int instead of uint for some silly things
static int64_t world_cam_x, world_cam_y;
static bool edit_tiles2 = false;
static bool edit_bgtiles = false;
static bool edit_renderone = false; // only one layer
static bool world_preview = false; // preview mode
static bool world_properties = false; // properties menu
static int8_t world_properties_option = 0;
static int32_t image_sel_option = 0;

void W_StartWorldEdit(const char *tileset_name, const char *world_name)
{
    if (world_name == NULL && tileset_name == NULL)
    {
        world_edit = true;
        file_menu = true;
        tile_sel_option = 0;
        W_UpdateFiles(&tileset_dirfiles, va("%s/data/tilesets", I_GetHomeDir())); // we need these too
        W_UpdateFiles(&world_dirfiles, va("%s/data/worlds", I_GetHomeDir()));
        W_UpdateFiles(&background_dirfiles, va("%s/data/backgrounds", I_GetHomeDir())); // and these
        return;
    }

    if (tileset_name == NULL) {
        W_LoadWorldFile(va("data/worlds/%s.wld", world_name));
        sprintf(temp_world_name, world_name);
    } else {
        W_CreateWorldFromTilesetFile(tileset_name, 32, 24);
        sprintf(temp_world_name, tileset_name);
    }

    tiles_per_row = gfx_tileset.width / tile_width; // waow
    tile_sel_option = 0; // i'm reusing EVERYTHING MWAHAHA
    current_tile = 0;
    world_cam_x = 0;
    world_cam_y = 0;
    edit_tiles2 = false;
    edit_bgtiles = false;
    edit_renderone = false;
    world_preview = false;
    world_properties = false;
    world_properties_option = 0;
    image_sel_option = 0;
    file_menu = false;
    world_edit = true;
}

void W_UpdateWorldEdit(void)
{  
    if (file_menu) { // in some way some how it's hackier than the tile one
        if (G_ControlDown(PLAYER_ONE, CON_START, true)) {
            file_menu = false;
            return;
        }

        if ((world_dirfiles.num_files + tileset_dirfiles.num_files) < 1)
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
            tile_sel_option = (world_dirfiles.num_files + tileset_dirfiles.num_files) - abs(tile_sel_option);

        // still below zero?
        if (tile_sel_option < 0)
            tile_sel_option = 0; // sigh..
        
        // loop around if need be
        tile_sel_option = tile_sel_option % (world_dirfiles.num_files + tileset_dirfiles.num_files);

        if (G_ControlDown(PLAYER_ONE, CON_A, true)) {
            char *dot;
            char *name;
            const char *filename;
            int name_len;

            if (tile_sel_option < world_dirfiles.num_files)
                filename = world_dirfiles.filenames[tile_sel_option];
            else
                filename = tileset_dirfiles.filenames[tile_sel_option - world_dirfiles.num_files];

            dot = strrchr(filename, '.');

            name_len = strlen(filename) + 1;
            name = malloc((name_len-4) * sizeof(char)); 
            snprintf(name, name_len-4, "%s", filename);

            if (!strcmp(dot, ".set"))
                W_StartWorldEdit(name, NULL);
            else if (!strcmp(dot, ".wld"))
                W_StartWorldEdit(NULL, name);

            free(name);
        }

        // return early
        return;
    }

    if (G_ControlDown(PLAYER_ONE, CON_START, true)) {
        W_StartWorldEdit(NULL, NULL); // open file menu
        return;
    }

    if (num_tiles <= 0)
        return;

    if (G_ControlDown(PLAYER_ONE, CON_SELECT, true)) {
        if (world_properties)
            world_properties = false;
        else
            world_properties = true;
        return;    
    }

    if (world_properties) {
        if (G_ControlDown(PLAYER_ONE, CON_LEFT, true)) {
            world_properties_option--;
            tile_sel_option = 0;
        }

        if (G_ControlDown(PLAYER_ONE, CON_RIGHT, true)) {
            world_properties_option++;
            tile_sel_option = 0;
        }

        // sanity checks
        if (world_properties_option < 0)
            world_properties_option = 3 - abs(world_properties_option);

        // still below zero?
        if (world_properties_option < 0)
            world_properties_option = 0; // sigh..
        
        // loop around if need be
        world_properties_option = world_properties_option % 3;

        if (world_properties_option == 0) {
            if (G_ControlDown(PLAYER_ONE, CON_UP, true))
                tile_sel_option--;
            if (G_ControlDown(PLAYER_ONE, CON_DOWN, true))
                tile_sel_option++;

            // sanity checks
            if (tile_sel_option < 0)
                tile_sel_option = BG_OPTIONS - abs(world_properties_option);

            // still below zero?
            if (tile_sel_option < 0)
                tile_sel_option = 0; // sigh..
            
            // loop around if need be
            tile_sel_option = tile_sel_option % BG_OPTIONS;

            if (G_ControlDown(PLAYER_ONE, CON_A, true))
                if (world_bgtype & (1<<tile_sel_option))
                    world_bgtype &= ~(1<<tile_sel_option);
                else
                    world_bgtype |= (1<<tile_sel_option);
        } 
        
        if (world_properties_option == 1 && background_dirfiles.num_files > 0) {
            if (G_ControlDown(PLAYER_ONE, CON_UP, true))
                image_sel_option--;
            if (G_ControlDown(PLAYER_ONE, CON_DOWN, true))
                image_sel_option++;

            // sanity checks
            if (image_sel_option < 0)
                image_sel_option = background_dirfiles.num_files - abs(world_properties_option);

            // still below zero?
            if (image_sel_option < 0)
                image_sel_option = 0; // sigh..
            
            // loop around if need be
            image_sel_option = image_sel_option % background_dirfiles.num_files;

            if (image_sel_option > background_dirfiles.num_files-1)
                image_sel_option = background_dirfiles.num_files-1;
            
            if (G_ControlDown(PLAYER_ONE, CON_A, true)) {
                GFX_FreeGFX(&gfx_worldbg);
                gfx_worldbg = GFX_LoadGFX(va("data/backgrounds/%s", background_dirfiles.filenames[image_sel_option]));
            }
        }

        if (world_properties_option == 2) {
            int8_t r, g, b;
            int8_t input = 0;

            r = (world_bgcolor >> 11) & 0x1F;
            g = (world_bgcolor >> 5) & 0x3F;
            b = world_bgcolor & 0x1F;

            if (G_ControlDown(PLAYER_ONE, CON_UP, true))
                tile_sel_option--;
            if (G_ControlDown(PLAYER_ONE, CON_DOWN, true))
                tile_sel_option++;
            if (G_ControlDown(PLAYER_ONE, CON_A, true))
                input--;
            if (G_ControlDown(PLAYER_ONE, CON_B, true))
                input++;

            // sanity checks
            if (tile_sel_option < 0)
                tile_sel_option = 3 - abs(world_properties_option);

            // still below zero?
            if (tile_sel_option < 0)
                tile_sel_option = 0; // sigh..
            
            // loop around if need be
            tile_sel_option = tile_sel_option % 3;

            if (tile_sel_option == 2)
                b += input;
            else if (tile_sel_option == 1)
                g += input;
            else
                r += input;

            if (r > 31)
                r = 31;
            if (g > 63)
                g = 63;
            if (b > 31)
                b = 31;
            if (r < 0)
                r = 0;
            if (g < 0)
                g = 0;
            if (b < 0)
                b = 0;

            world_bgcolor = (r << 11) | (g << 5) | b;
        }

        if (background_dirfiles.num_files <= 0)
            world_bgtype &= ~BG_IMAGE;

        return;
    }

    if (world_preview) {
        if (G_ControlDown(PLAYER_ONE, CON_UP, false))
            camera.y--;
        if (G_ControlDown(PLAYER_ONE, CON_DOWN, false))
            camera.y++;
        if (G_ControlDown(PLAYER_ONE, CON_LEFT, false))
            camera.x--;
        if (G_ControlDown(PLAYER_ONE, CON_RIGHT, false))
            camera.x++;
        if (G_ControlDown(PLAYER_ONE, CON_B, false))
            world_preview = false;

        return;
    }

    if (G_ControlDown(PLAYER_ONE, CON_UP, true))
        current_tile-=tiles_per_row;
    if (G_ControlDown(PLAYER_ONE, CON_DOWN, true))
        current_tile+=tiles_per_row;
    if (G_ControlDown(PLAYER_ONE, CON_LEFT, true))
        current_tile--;
    if (G_ControlDown(PLAYER_ONE, CON_RIGHT, true))
        current_tile++;

    // sanity checks
    if (current_tile < 0)
        current_tile = num_tiles - abs(current_tile);

    // still below zero?
    if (current_tile < 0)
        current_tile = 0; // sigh..
     
    // loop around if need be
    current_tile = current_tile % num_tiles;

    // camera
    if (G_ControlDown(PLAYER_ONE, CON_A, false))
        world_cam_x-=2;
    if (G_ControlDown(PLAYER_ONE, CON_B, false))
        world_cam_y+=2;
    if (G_ControlDown(PLAYER_ONE, CON_C, false))
        world_cam_x+=2;
    if (G_ControlDown(PLAYER_ONE, CON_Y, false))
        world_cam_y-=2;

    // camera bounds
    if (world_cam_x < 0)
        world_cam_x = 0;
    if (world_cam_y < 0)
        world_cam_y = 0;
    if (world_cam_x > world_width*tile_width*2 - VID_WIDTH)
        world_cam_x = world_width*tile_width*2 - VID_WIDTH;
    if (world_cam_y > world_height*tile_height*2 - VID_HEIGHT + 21)
        world_cam_y = world_height*tile_height*2 - VID_HEIGHT + 21;

    if (G_ControlDown(PLAYER_ONE, CON_X, true))
        if (edit_tiles2) {
            if (edit_bgtiles)
                edit_bgtiles = false;
            else 
                edit_bgtiles = true;

            edit_tiles2 = false;
        } else
            edit_tiles2 = true;

    if (G_ControlDown(PLAYER_ONE, CON_Z, true))
        if (edit_renderone)
            edit_renderone = false;
        else
            edit_renderone = true;

    if (G_MouseControlDown(PLAYER_ONE, MOUSE_LBUTTON, false)) {
        int16_t mouse_tilex, mouse_tiley;

        mouse_tilex = (G_MouseAxis(PLAYER_ONE, MOUSE_POSX)+world_cam_x) / (tile_width*2);
        mouse_tiley = (G_MouseAxis(PLAYER_ONE, MOUSE_POSY)+world_cam_y - 41) / (tile_height*2);
        
        if (mouse_tilex < 0)
            mouse_tilex = 0;
        if (mouse_tilex > world_width-1)
            mouse_tilex = world_width-1;
        if (mouse_tiley < 0)
            mouse_tiley = 0;
        if (mouse_tiley > world_height-1)
            mouse_tiley = world_height-1;

        if (edit_bgtiles)
            if (edit_tiles2)
                world_bgtiles2[mouse_tilex + mouse_tiley*world_width] = current_tile;
            else
                world_bgtiles[mouse_tilex + mouse_tiley*world_width] = current_tile;
        else
            if (edit_tiles2)
                world_tiles2[mouse_tilex + mouse_tiley*world_width] = current_tile;
            else
                world_tiles[mouse_tilex + mouse_tiley*world_width] = current_tile;
    }
}

// putting these here for more lazy
int16_t tile_offy = 0;
int16_t tile_offx = 0;

// my super yucky drawing code x2
void W_DrawWorldEdit(void)
{
    int i, cur_posx, cur_posy;

    if (file_menu) {
        V_DrawText("Nozomi Engine World Editor\nSelect a file to open:", 0, 0, 0);

        if (world_dirfiles.num_files + tileset_dirfiles.num_files > 0) {
            for (i = 0; i < world_dirfiles.num_files + tileset_dirfiles.num_files; i++) {
                if (i < world_dirfiles.num_files)
                    V_DrawText(va("%s", world_dirfiles.filenames[i]), (i/17) * 80 + 8, (i%17) * 10 + 20, 0);
                else {
                    char *dot;
                    dot = strrchr(tileset_dirfiles.filenames[i - world_dirfiles.num_files], '.');

                    if (!strcmp(dot, ".bmp"))
                        V_DrawText(va("N/A - %s", tileset_dirfiles.filenames[i - world_dirfiles.num_files]), (i/17) * 80 + 8, (i%17) * 10 + 20, 0);
                    else
                        V_DrawText(va("%s", tileset_dirfiles.filenames[i - world_dirfiles.num_files]), (i/17) * 80 + 8, (i%17) * 10 + 20, 0);
                }        
            }

            V_DrawText(">", (tile_sel_option/17) * 80 + 1, (tile_sel_option%17) * 10 + 20, 0);
        } else
            V_DrawText("No files found.", 8, 20, 0);

        V_DrawText("Close the File Menu with Start/Enter", 0, VID_HEIGHT-8, 0);
        // return early
        return;
    }

    if (num_tiles <= 0) { // technically can still do this lolsies
        V_DrawText("No world loaded.\nRe-open the File Menu with Start/Enter.", 0, 0, 0);
        return;
    }

    if (world_properties) {
        char *dot;

        V_DrawText(va("Background Type:         %02d", world_bgtype), 2, 2, 0);

        if (world_properties_option == 0)
            V_DrawText(">", 2, 12 + tile_sel_option*10, 0);
        else if (world_properties_option == 1)
            V_DrawText("U/D:", VID_WIDTH - 152, 12, 0);
        else if (world_properties_option == 2)
            V_DrawText(">", VID_WIDTH - 128, 31 + tile_sel_option*10, 0);

        V_DrawText("Background Image:", VID_WIDTH - 128, 2, 0);
        if (background_dirfiles.num_files <= 0)
            V_DrawText("No images found.", VID_WIDTH - 128, 12, 0);
        else {
            char *dot;
            char *name;
            const char *filename;
            int name_len;

            filename = background_dirfiles.filenames[image_sel_option];
            dot = strrchr(filename, '.');

            name_len = strlen(filename) + 1;
            name = malloc((name_len-4) * sizeof(char)); 
            snprintf(name, name_len-4, "%s", filename);

            V_DrawText(va("%s.bmp", name), VID_WIDTH - 128, 12, 0);

            free(name);
        }

        V_DrawText("Background Color:", VID_WIDTH - 128, 22, 0);
        for (i = 0; i < 64; i++) {
            V_DrawDot(VID_WIDTH - 128 + i + 16, 35, (i/2 << 11));
            V_DrawDot(VID_WIDTH - 128 + i + 16, 45, (i/2 << 5));
            V_DrawDot(VID_WIDTH - 128 + i + 16, 55, i/2);
            V_DrawDot(VID_WIDTH - 128 + i + 16, 36, (i/2 << 11));
            V_DrawDot(VID_WIDTH - 128 + i + 16, 46, (i/2 << 5));
            V_DrawDot(VID_WIDTH - 128 + i + 16, 56, i/2);
        }

        {
            uint8_t r, g, b;
            r = (world_bgcolor >> 11) & 0x1F;
            g = (world_bgcolor >> 5) & 0x3F;
            b = world_bgcolor & 0x1F;

            V_DrawText("|", VID_WIDTH - 128 + r*2 + 14, 32, 0);
            V_DrawText("|", VID_WIDTH - 128 + g + 14, 42, 0);
            V_DrawText("|", VID_WIDTH - 128 + b*2 + 14, 52, 0);
        }

        if (world_bgtype & BG_STATIC) 
            V_DrawText("Static", 12, 12, V_JUMPYTEXT);
        else
            V_DrawText("Static", 12, 12, 0);

        if (world_bgtype & BG_WATER)
            V_DrawText("Water", 12, 22, V_JUMPYTEXT);
        else
            V_DrawText("Water", 12, 22, 0);

        if (world_bgtype & BG_FG)
            V_DrawText("Foreground", 12, 32, V_JUMPYTEXT);
        else
            V_DrawText("Foreground", 12, 32, 0);

        if (world_bgtype & BG_SLOW)
            V_DrawText("Slow Parallax", 12, 42, V_JUMPYTEXT);
        else
            V_DrawText("Slow Parallax", 12, 42, 0);

        if (world_bgtype & BG_COLOR)
            V_DrawText("Color", 12, 52, V_JUMPYTEXT);
        else
            V_DrawText("Color", 12, 52, 0);

        if (world_bgtype & BG_IMAGE)
            V_DrawText("Image", 12, 62, V_JUMPYTEXT);
        else
            V_DrawText("Image", 12, 62, 0);

        if (world_bgtype & BG_SCROLL)
            V_DrawText("Scroll", 12, 72, V_JUMPYTEXT);
        else
            V_DrawText("Scroll", 12, 72, 0);

        return;
    }

    if (world_preview) {
        W_DrawWorld();

        V_DrawText("WORLD PREVIEW", 2, VID_HEIGHT - 20, 0);
        V_DrawText("D-Pad: Move Cam. | B: Return to Editor", 2, VID_HEIGHT - 10, 0);
        return;
    }

    cur_posx = 2 + (current_tile % tiles_per_row) * (tile_width+1)*2;
    cur_posy = 2 + (current_tile / tiles_per_row) * (tile_height+1)*2;

    if (cur_posx > ((tiles_per_row)/4) * (2*tile_width+2))
        tile_offx = (cur_posx/(2*tile_width+2)) - ((tiles_per_row)/4);
    else 
        tile_offx = 0;

    if (tile_offx >= tiles_per_row - (tiles_per_row)/2)
        tile_offx = tiles_per_row - (tiles_per_row)/2;

    if (cur_posy > 2*tile_height+2)
        tile_offy = (cur_posy/(2*tile_height+2)) - 1;
    else 
        tile_offy = 0;

    if (tile_offy >= num_tiles/tiles_per_row - 2)
        tile_offy = num_tiles/tiles_per_row - 2;

    // draw world bg color before tiles
    if ((world_bgtype & BG_COLOR) && (world_bgtype & BG_WATER) == 0)
        V_FillScreen(world_bgcolor);

    // draw world bg image after color and before tiles
    if (world_bgtype & BG_IMAGE)
        V_DrawTiled(gfx_worldbg, -world_cam_x, -world_cam_y, 0);

    // draw world tiles first
    for (i = 0; i < world_width*world_height; i++)
    {
        int32_t px, py;
        uint32_t id3 = world_bgtiles[i];
        uint32_t id4 = world_bgtiles2[i];

        px = ((i % world_width) * tile_width) - world_cam_x/2;
        py = ((i / world_width) * tile_height) - world_cam_y/2;

        if ((edit_renderone && edit_tiles2 && edit_bgtiles) || !edit_renderone)
            if (id4 != 0)
                V_DrawCropped2x(
                    gfx_tileset, // gfx
                    px, // x 
                    21 + py, // y
                    (id4 % tiles_per_row) * tile_width, // crop x
                    (id4 / tiles_per_row) * tile_height, // crop y
                    tile_width, // crop w
                    tile_height, // crop h
                    0 // flags
                );

        if ((edit_renderone && edit_tiles2 == false && edit_bgtiles) || !edit_renderone)
            if (id3 != 0)
                V_DrawCropped2x(
                    gfx_tileset, // gfx
                    px, // x 
                    21 + py, // y
                    (id3 % tiles_per_row) * tile_width, // crop x
                    (id3 / tiles_per_row) * tile_height, // crop y
                    tile_width, // crop w
                    tile_height, // crop h
                    0 // flags
                );
    }

    if ((world_bgtype & BG_FG) == 0 && (world_bgtype & BG_WATER))
        W_DrawWaveEffect(world_bgcolor);

    for (i = 0; i < world_width*world_height; i++)
    {
        int32_t px, py;
        uint32_t id = world_tiles[i];
        uint32_t id2 = world_tiles2[i];

        px = ((i % world_width) * tile_width) - world_cam_x/2;
        py = ((i / world_width) * tile_height) - world_cam_y/2;

        if ((edit_renderone && edit_tiles2 && edit_bgtiles == false) || !edit_renderone)
            if (id2 != 0)
                V_DrawCropped2x(
                    gfx_tileset, // gfx
                    px, // x 
                    21 + py, // y
                    (id2 % tiles_per_row) * tile_width, // crop x
                    (id2 / tiles_per_row) * tile_height, // crop y
                    tile_width, // crop w
                    tile_height, // crop h
                    0 // flags
                );

        if ((edit_renderone && edit_tiles2 == false && edit_bgtiles == false) || !edit_renderone)
            if (id != 0)
                V_DrawCropped2x(
                    gfx_tileset, // gfx
                    px, // x 
                    21 + py, // y
                    (id % tiles_per_row) * tile_width, // crop x
                    (id / tiles_per_row) * tile_height, // crop y
                    tile_width, // crop w
                    tile_height, // crop h
                    0 // flags
                );
    }

    if ((world_bgtype & (BG_FG|BG_WATER)) == (BG_FG|BG_WATER))
        W_DrawWaveEffect(world_bgcolor);

    // directly writing to the buffer is bad but i need to
    {
        int x, y;
        for (y = 0; y < 42; y++)
            for (x = 0; x < VID_WIDTH; x++)
            vid.buffer[x + y*VID_WIDTH] = 0;
        for (y = VID_HEIGHT-8; y < VID_HEIGHT; y++)
            for (x = 0; x < VID_WIDTH; x++)
            vid.buffer[x + y*VID_WIDTH] = 0;
    }

    V_DrawText("D-Pad: Change Tile | Mouse: Place | YABC: Move Cam.", 2, VID_HEIGHT-8, 0);
    V_DrawText("Start: File Menu | Select: World Properties", 2, VID_HEIGHT-18, 0);

    if (edit_bgtiles)
        if (edit_tiles2)
            V_DrawText("BG Layer 2", 2, VID_HEIGHT-28, 0);
        else
            if (world_bgtype & BG_FG)
                V_DrawText("FG Layer", 2, VID_HEIGHT-28, 0);
            else
                V_DrawText("BG Layer 1", 2, VID_HEIGHT-28, 0);
    else
        if (edit_tiles2)
            V_DrawText("Layer 2", 2, VID_HEIGHT-28, 0);
        else
            V_DrawText("Layer 1", 2, VID_HEIGHT-28, 0);

    if (edit_renderone)
        V_DrawText("Render One", 120, VID_HEIGHT-28, 0);
    else
        V_DrawText("Render All", 120, VID_HEIGHT-28, 0);

    // tile selector
    for (i = 0; i < num_tiles; i++)
    {
        int32_t px, py;

        px = VID_WIDTH/4 - (tile_width+1) + (i % tiles_per_row) * (tile_width + 1) - (tile_offx * (tile_width+1));
        py = 2 + (i / tiles_per_row) * (tile_height + 1) - (tile_offy * (tile_height+1));

        if (px < VID_WIDTH/4 - tile_width*2 || py < 0)
            continue;

        if (i/tiles_per_row - tile_offy > 1)
            break;

        if (i%tiles_per_row - tile_offx > tiles_per_row/2 - 1)
            continue;

        V_DrawCropped2x(
            gfx_tileset, // gfx
            px, // x 
            py, // y
            (i % tiles_per_row) * tile_width, // crop x
            (i / tiles_per_row) * tile_height, // crop y
            tile_width, // crop w
            tile_height, // crop h
            0 // flags
        );
    }

    V_DrawBox(VID_WIDTH/2 + cur_posx - (tile_offx * (2*tile_width+2)) - tile_width*2 - tile_width/2 - 1, cur_posy - (tile_offy * (2*tile_height+2)) + 1, 0, 2*tile_width + 2, 2*tile_height + 2, 0xFFFF);
    V_DrawLine(0, 42, 90, VID_WIDTH, 0xFFFF);
    V_DrawLine(VID_WIDTH/2 - tile_width*2 - (tile_width-1), 0, 180, 42, 0xFFFF);

    if (tile_attributes[current_tile] & TILE_SOLID) 
        V_DrawText("Solid", 2, 3, V_JUMPYTEXT);
    else
        V_DrawText("Solid", 2, 3, 0);

    if (tile_attributes[current_tile] & TILE_WATER) 
        V_DrawText("Water", 2, 12, V_JUMPYTEXT);
    else
        V_DrawText("Water", 2, 12, 0);

    if (tile_attributes[current_tile] & TILE_FG) 
        V_DrawText("Foreground", 2, 21, V_JUMPYTEXT);
    else
        V_DrawText("Foreground", 2, 21, 0);

    if (tile_attributes[current_tile] & TILE_ANIMATED) 
        V_DrawText("Animated", 2, 30, V_JUMPYTEXT);
    else
        V_DrawText("Animated", 2, 30, 0);
}

bool tileset_edit = false; // tileset editing dawg
static char temp_tile_name[33]; // save temp tileset to this name

static int16_t tile_screenw = 15;
static int16_t tile_screenh = 11;
static bool tile_selected = false;

// one or the other
void W_StartTilesetEdit(const char *gfx_name, const char *tileset_name)
{
    if (gfx_name == NULL && tileset_name == NULL)
    {
        tileset_edit = true;
        file_menu = true;
        tile_sel_option = 0;
        W_UpdateFiles(&tileset_dirfiles, va("%s/data/tilesets", I_GetHomeDir()));
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
        int32_t px, py;

        px = 2 + (i % tiles_per_row) * (tile_width + 1) - (tile_offx * (tile_width+1));
        py = 2 + (i / tiles_per_row) * (tile_height + 1) - (tile_offy * (tile_height+1));

        if (px < 0 || py < 0)
            continue;

        if (i/tiles_per_row - tile_offy > tile_screenh-1)
            break;

        if (i%tiles_per_row - tile_offx > tile_screenw-1)
            continue;

        V_DrawCropped(
            gfx_tileset, // gfx
            px, // x 
            py, // y
            (i % tiles_per_row) * tile_width, // crop x
            (i / tiles_per_row) * tile_height, // crop y
            tile_width, // crop w
            tile_height, // crop h
            0 // flags
        );
    }

    V_DrawLine(tile_screenw * (tile_height + 1) + 3, 0, 180, tile_screenh * (tile_height + 1) + 4, 0xFFFF);
    V_DrawLine(0, tile_screenh * (tile_height + 1 ) + 3, 90, tile_screenw * (tile_height + 1) + 4, 0xFFFF);

    V_DrawBox(cur_posx - (tile_offx * (tile_width+1)), cur_posy - (tile_offy * (tile_height+1)), 0, tile_width + 2, tile_height + 2, 0xFFFF);
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

    V_DrawText("Solid", VID_WIDTH-108, 30, 0);
    V_DrawText("Water", VID_WIDTH-108, 42, 0);
    V_DrawText("Foreground", VID_WIDTH-108, 54, 0);
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