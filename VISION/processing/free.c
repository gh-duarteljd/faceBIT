#include "processing.h"

void
free_image(image input_image)
{
	for (int i = 0; i < input_image.height; i++)
	{
		free(input_image.pixels_map[i]);
	}
	free(input_image.pixels_map);
}
