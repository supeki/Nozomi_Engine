#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main(void)
{
	FILE *fp = fopen("default.fnt", "wb");
	char gfx_name[33];
	bool bitmap = false;
	int16_t offset[256];
	uint16_t size[256];
	uint16_t charsize = 0x0808;
	
	snprintf(gfx_name, 32, "default");
	fwrite(gfx_name, sizeof(gfx_name)-1, 1, fp);
	fwrite(&bitmap, sizeof(bool), 1, fp);
	fwrite(&charsize, sizeof(uint16_t), 1, fp);

	for (int i = 0; i < 256; i++)
		offset[i] = 0x0000;
	for (int i = 0; i < 256; i++)
		size[i] = 0x0808;

	fwrite(&offset, sizeof(offset), 1, fp);
	fwrite(&size, sizeof(size), 1, fp);

	fclose(fp);

	return 0;
}