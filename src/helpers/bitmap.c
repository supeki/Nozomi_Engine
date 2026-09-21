#include "bitmap.h"

#include "../game_defs.h"
#include "../i_system.h"

bitmap_t Bitmap_Load(const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	bitmap_t bitmap;
	BMP_Header_t header;
	BMP_DIBHeader_t dib_header;
	int i;

	memset(&bitmap, 0, sizeof(bitmap_t));

	if (!fp)
		I_Error("Failed to find bitmap file: %s", filename);
	
	fread(&header.ident, sizeof(char), 2, fp);
	header.size = FIL_ReadU32(fp);
	header.reserved_1 = FIL_ReadU16(fp);
	header.reserved_2 = FIL_ReadU16(fp);
	header.data_offset = FIL_ReadU32(fp);
	header.ident[2] = '\0';
	
	if (strcmp(header.ident, "BM") != 0)
	{
		I_printf("Invalid BMP identifier: %s\nExpected: BM\n", header.ident);
		fclose(fp);
		return bitmap;
	}
	
	/*
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
	*/

	memset(&dib_header, 0, sizeof(BMP_DIBHeader_t));
	dib_header.size = FIL_ReadU32(fp);
	dib_header.width = FIL_ReadS32(fp);
	dib_header.height = FIL_ReadS32(fp);
	dib_header.color_planes = FIL_ReadU16(fp);
	dib_header.bpp = FIL_ReadU16(fp);
	dib_header.compression = FIL_ReadU32(fp);
	dib_header.image_size = FIL_ReadU32(fp);
	dib_header.horiz_res = FIL_ReadU32(fp);
	dib_header.verti_res = FIL_ReadU32(fp);
	dib_header.num_colors = FIL_ReadU32(fp);
	dib_header.imp_colors = FIL_ReadU32(fp);
	
	if (dib_header.compression > BI_RLE4)
	{
		I_printf("Unsupported BMP type!\nBit fields are not supported.\n\nThis isn't your fault, I'm just lazy.\n");
		fclose(fp);
		return bitmap;
	}
	
	bitmap.width = dib_header.width;
	bitmap.height = dib_header.height;
	bitmap.bpp = dib_header.bpp;
	bitmap.compression = dib_header.compression;
	bitmap.pixel_data = malloc(dib_header.image_size);
	
	if (bitmap.bpp <= 8) { // >= 16bpp stores color data in the pixel data
 		bitmap.palette = malloc(dib_header.num_colors*sizeof(rgb_t));
	
		for (i = 0; i < dib_header.num_colors; i++)
		{
			uint8_t unused;
			
			memset(&bitmap.palette[i], 0, sizeof(rgb_t));
			fread(&bitmap.palette[i].b, sizeof(uint8_t), 1, fp);
			fread(&bitmap.palette[i].g, sizeof(uint8_t), 1, fp);
			fread(&bitmap.palette[i].r, sizeof(uint8_t), 1, fp);
			fread(&unused, sizeof(uint8_t), 1, fp); // alpha (unused)
		}
	}
	
	fseek(fp, header.data_offset, SEEK_SET);
	fread(bitmap.pixel_data, sizeof(uint8_t), dib_header.image_size, fp);
	fclose(fp);
	
	return bitmap;
}

void Bitmap_Free(bitmap_t *bitmap)
{
	bitmap->width = 0;
	bitmap->height = 0;
	bitmap->bpp = 0;
	bitmap->compression = 0;

	if (bitmap->pixel_data != NULL) {
		free(bitmap->pixel_data);
		bitmap->pixel_data = NULL;
	}

	if (bitmap->palette != NULL) {
		free(bitmap->palette);
		bitmap->palette = NULL;
	}
}
