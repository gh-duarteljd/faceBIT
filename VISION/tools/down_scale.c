#include "tools.h"

image
down_scale(image input_image, float scale)
{
	image buffer;
	buffer.width = input_image.width / scale;
	buffer.height = input_image.height / scale;

	buffer.pixels_map = (float**)malloc(buffer.height * sizeof(float*));
	for (int i = 0; i < buffer.height; i++)
	{
		buffer.pixels_map[i] = (float*) malloc(buffer.width * sizeof(float));

		for (int j = 0; j < buffer.width; j++)
		{
			int x = j * scale;
			int y = i * scale;
			buffer.pixels_map[i][j] = input_image.pixels_map[y][x];
		}
	}

	return buffer;
}
