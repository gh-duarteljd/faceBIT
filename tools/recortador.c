#include "../VISION/VISION.h"

int main(int argc, char** argv)
{
  // ensure proper usage
	if (argc != 2)
	{
			printf("Usage: ./recortador input_image \n");
			exit(1);
	}

	image input_image = load_image(argv[1]);

	int w = input_image.width / 168;
	int h = input_image.height / 192;

	image buffer;
	buffer.height = 192;
	buffer.width = 168;

	// Mapa de pixels.
	buffer.pixels_map = malloc(buffer.height * sizeof(double*));
	for (int i = 0; i < buffer.height; i++)
	{
		buffer.pixels_map[i] = malloc(buffer.width * sizeof(double));
	}

	for (int i = 0, o = 986; i < h; i++)
	{
		for (int j = 0; j < w; j++, o ++)
    {
			for (int k = (i * 192), m = 0; k < (i * 192 + 192); k++, m++)
			{
				for (int l = (j * 168), n = 0; l < (j * 168 + 168); l++, n++)
				{
					buffer.pixels_map[m][n] = input_image.pixels_map[k][l];
				}
			}
			char path[30];
	    sprintf(path, "nofaces/%d.pgm", o + 1);
			save_image(buffer, path);
		}
	}


free_image(input_image);
}
