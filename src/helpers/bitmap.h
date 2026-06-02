#ifndef BITMAP_H
#define BITMAP_H

#include "../game_defs.h"

typedef struct
{
	char ident[3];		  // should probably just be 'BM' right?
	uint32_t size;  	  // size of the BMP file
	uint16_t reserved_1;  // unk
	uint16_t reserved_2;  // unk
	uint32_t data_offset; // offset from start of file to the pixel data
} BMP_Header_t;

typedef struct
{
	uint32_t size;		   // header size
	int32_t width; 		   // image width
	int32_t height; 	   // image height
	uint16_t color_planes; // number of color planes
	uint16_t bpp;		   // bits per pixel
	uint32_t compression;  // compression type
	uint32_t image_size;   // size of the raw bitmap data
	uint32_t horiz_res;	   // i shouldn't need this right
	uint32_t verti_res;	   // or this either right
	uint32_t num_colors;   // number of colors in the color palette
	uint32_t imp_colors;   // "important" colors or something
} BMP_DIBHeader_t;

#ifndef WINDOWS
typedef enum
{
	BI_RGB,
	BI_RLE8,
	BI_RLE4,
	BI_BITFIELDS,
	BI_JPEG,
	BI_PNG,
	BI_ALPHABITFIELDS,
	BI_CMYK,
	BI_CMYKRLE8,
	BI_CMYKRLE4,
} compression_types_e;
#endif

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgb_t;

typedef struct
{
	int32_t width;
	int32_t height;
	uint16_t bpp;
	uint32_t compression;
	uint8_t *pixel_data;
	rgb_t *palette;
} bitmap_t;

bitmap_t Bitmap_Load(const char *filename);

#endif