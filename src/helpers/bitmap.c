#include "bitmap.h"

#include "../game_defs.h"
#include "../i_system.h"

bitmap_t Bitmap_Load(const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	BMP_Header_t header;
	BMP_DIBHeader_t dib_header;
	bitmap_t bitmap;
	
	fread(&header.ident, sizeof(char), 2, fp);
	fread(&header.size, sizeof(uint32_t), 1, fp);
	fread(&header.reserved_1, sizeof(uint16_t), 1, fp);
	fread(&header.reserved_2, sizeof(uint16_t), 1, fp);
	fread(&header.data_offset, sizeof(uint32_t), 1, fp);
	header.ident[2] = '\0';
	
	if (strcmp(header.ident, "BM") != 0)
	{
		I_printf("Invalid BMP identifier: %s\nExpected: BM\n", header.ident);
		fclose(fp);
		return bitmap;
	}
	
	fread(&dib_header, sizeof(BMP_DIBHeader_t), 1, fp);
	
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
	
		for (int i = 0; i < dib_header.num_colors; i++)
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