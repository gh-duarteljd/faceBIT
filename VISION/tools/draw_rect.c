#include "tools.h"

void
draw_rect(image input_image,
					int 	x,
					int 	y,
					int 	w,
					int 	h,
					int 	pixels_width,
					int 	color)
{
	if ((x > input_image.width) || (y > input_image.height) || (x < 0) || (y < 0))
	{
		fprintf(stderr, "Could not draw rectangle.\n");
    exit(2);
	}

	int x_inicial = x - w / 2 - pixels_width;
	if (x_inicial < 0)
	{
		x_inicial = 0;
	}

	int x_final = x + w / 2 + pixels_width;
	if (x_final > input_image.width)
	{
		x_final = input_image.width;
	}

	int y_inicial = y - h / 2 - pixels_width;
	if (y_inicial < 0)
	{
		y_inicial = 0;
	}

	int y_final = y + h / 2 + pixels_width;
	if (y_final > input_image.height)
	{
		y_final = input_image.height;
	}

	for (int i = y_inicial; i < y_final; i++)
	{
		for (int j = x_inicial; j < x_final; j++)
		{
			if ((i < (y_inicial + pixels_width)) || (i >= (y_final - pixels_width)) || (j < (x_inicial + pixels_width)) || (j >= (x_final - pixels_width)))
			{
				input_image.pixels_map[i][j] = color;
			}
		}
	}
	return;
}
